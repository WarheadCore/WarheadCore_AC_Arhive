/*
 * This file is part of the WarheadCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Log.h"
#include "ScriptMgr.h"
#include "GameConfig.h"
#include "Chat.h"
#include "Player.h"
#include "DatabaseEnv.h"
#include "StringFormat.h"
#include <map>

struct RewardPlayedTime
{
    uint32 ItemID;
    uint32 Count;
};

enum TypeReward
{
    DEFAULT_REWARD = 1,
    REWARD_PER_HOUR
};

class OnlineReward
{
public:
    static OnlineReward* instance()
    {
        static OnlineReward instance;
        return &instance;
    }

    void SetInterval()
    {
        updateTimer.SetInterval(CONF_GET_INT("OnlineReward.CheckTime") * IN_MILLISECONDS);
    }

    void LoadDataFromDB()
    {
        uint32 msTime = getMSTime();
        rewards.clear();

        LOG_INFO("module", "Loading online rewards...");

        QueryResult result = CharacterDatabase.Query("SELECT RewardPlayedTime, ItemID, Count FROM online_reward");
        if (!result)
        {
            sLog->outErrorDb(">> In DB table `online_reward` not data. Loading canceled");
            sLog->outString();
            return;
        }

        do
        {
            Field* fields = result->Fetch();

            uint32 Seconds = fields[0].GetUInt32();

            RewardPlayedTime RPT;
            RPT.ItemID = fields[1].GetUInt32();
            RPT.Count = fields[2].GetUInt32();

            // Проверка
            if (Seconds == 0)
            {
                sLog->outErrorDb("-> Time = 0? Really? Skip...");   
                continue;
            }

            ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(RPT.ItemID);
            if (!itemTemplate)
            {
                sLog->outErrorDb("-> Item with number %u not found. Skip", RPT.ItemID);    
                continue;
            }

            if (RPT.Count <= 0)
            {
                sLog->outErrorDb("-> Item count for number %u - %u. Set to 1", RPT.ItemID, RPT.Count);     
                RPT.Count = 1;
            }

            rewards.insert(std::make_pair(Seconds, RPT));

        } while (result->NextRow());

        sLog->outString(">> Loaded %u reward in %u ms", (uint32)rewards.size(), GetMSTimeDiffToNow(msTime));
        sLog->outString();
    }

    void RewardPlayer(Player* player)
    {
        if (CONF_GET_BOOL("OnlineReward.PerOnline.Enable"))
            RewardPerOnline(player);

        if (CONF_GET_BOOL("OnlineReward.PerTime.Enable"))
            RewardPerTime(player);
    }

    void LoadRewardForPlayer(Player* player)
    {
        if (CONF_GET_BOOL("OnlineReward.PerOnline.Enable"))
            LoadRewardFromDB(player, true);

        if (CONF_GET_BOOL("OnlineReward.PerTime.Enable"))
            LoadRewardFromDB(player, false);
    }

    void SaveRewardForPlayer(Player* player)
    {
        if (CONF_GET_BOOL("OnlineReward.PerOnline.Enable"))
            _lastrewardtimestore.erase(player->GetGUIDLow());

        if (CONF_GET_BOOL("OnlineReward.PerTime.Enable"))
            _LastRewardTimePerTimeStore.erase(player->GetGUIDLow());
    }

    IntervalTimer& GetUpdateTimer() { return updateTimer; }

private:
    void RewardPerOnline(Player* player)
    {
        ChatHandler handler(player->GetSession());
        uint32 PlayedTimeSec = player->GetTotalPlayedTime();
        uint64 PlayerGuid = player->GetGUIDLow();

        auto SendMailItems = [](Player* player, std::string Subject, std::string Text, uint32 Money, MailItemsVector ListItemPairs)
        {
            if (ListItemPairs.size() > MAX_MAIL_ITEMS)
            {
                sLog->outError("> SendMailItems: ListItemPairs.size() = %u", (uint32)ListItemPairs.size());
                return;
            }

            MailItemsVector _listItemPairs;

            // check
            for (auto itr : ListItemPairs)
            {
                uint32 ItemID = itr.first;
                uint32 ItemCount = itr.second;

                ItemTemplate const* item = sObjectMgr->GetItemTemplate(ItemID);
                if (!item)
                    continue;

                if (ItemCount < 1 || (item->MaxCount > 0 && ItemCount > uint32(item->MaxCount)))
                    break;

                while (ItemCount > item->GetMaxStackSize())
                {
                    _listItemPairs.push_back(MailItemsPair(ItemID, item->GetMaxStackSize()));
                    ItemCount -= item->GetMaxStackSize();
                }

                _listItemPairs.push_back(MailItemsPair(ItemID, ItemCount));

                if (_listItemPairs.size() > MAX_MAIL_ITEMS)
                {
                    sLog->outError("> SendMailItems: _listItemPairs.size() = %u", (uint32)_listItemPairs.size());
                    break;
                }
            }

            // from console show not existed sender
            MailSender sender(MAIL_NORMAL, player->GetGUIDLow(), MAIL_STATIONERY_DEFAULT);

            SQLTransaction trans = CharacterDatabase.BeginTransaction();

            // fill mail
            MailDraft draft(Subject, Text);

            for (auto itr : _listItemPairs)
            {
                if (Item* item = Item::CreateItem(itr.first, itr.second, player))
                {
                    item->SaveToDB(trans);                               // save for prevent lost at next mail load, if send fail then item will deleted
                    draft.AddItem(item);
                }
            }

            if (Money)
                draft.AddMoney(Money);

            draft.SendMailTo(trans, player, sender);
            CharacterDatabase.CommitTransaction(trans);
        };

        for (auto const& itr : rewards)
        {
            auto Seconds = itr.first;
            auto ItemID = itr.second.ItemID;
            auto Count = itr.second.Count;
            bool IsCompleteReward = false;

            if (IsRewardedPlayer(PlayerGuid, Seconds))
                continue;

            if (PlayedTimeSec > Seconds)
                IsCompleteReward = true;

            if (IsCompleteReward)
            {
                ChatHandler handler(player->GetSession());
                std::string Subject, Text, SelfMessage;
                std::string PlayedTimeSecStr = secsToTimeString(Seconds);

                MailItemsVector ListItemPairs;
                ListItemPairs.push_back(MailItemsPair(ItemID, Count));

                Subject = warhead::StringFormat("Reward for online %s", PlayedTimeSecStr.c_str());
                Text = warhead::StringFormat("Привет, %s!\nВы играете на нашем сервере уже более %s. Пожалуйста примите подарок", player->GetName().c_str(), PlayedTimeSecStr.c_str());
                SelfMessage = warhead::StringFormat("You were rewarded for online (%s). You can get the award at the post office.", PlayedTimeSecStr.c_str());

                handler.PSendSysMessage("%s", SelfMessage.c_str());
                SendMailItems(player, Subject, Text, 0, ListItemPairs);

                _lastrewardtimestore[PlayerGuid].push_back(Seconds);
            }
        }

        SaveRewardDB(player, true);
    }

    void RewardPerTime(Player* player)
    {
        ChatHandler handler(player->GetSession());
        uint32 PlayedTimeSec = player->GetTotalPlayedTime();
        uint32 PlayerGuid = player->GetGUIDLow();
        uint32 LastReward = _LastRewardTimePerTimeStore[PlayerGuid];
        uint32 DiffTime = 0;

        auto SendMailItems = [](Player* player, std::string Subject, std::string Text, uint32 Money, MailItemsVector ListItemPairs)
        {
            if (ListItemPairs.size() > MAX_MAIL_ITEMS)
            {
                sLog->outError("> SendMailItems: ListItemPairs.size() = %u", (uint32)ListItemPairs.size());
                return;
            }

            MailItemsVector _listItemPairs;

            // check
            for (auto itr : ListItemPairs)
            {
                uint32 ItemID = itr.first;
                uint32 ItemCount = itr.second;

                ItemTemplate const* item = sObjectMgr->GetItemTemplate(ItemID);
                if (!item)
                    continue;

                if (ItemCount < 1 || (item->MaxCount > 0 && ItemCount > uint32(item->MaxCount)))
                    break;

                while (ItemCount > item->GetMaxStackSize())
                {
                    _listItemPairs.push_back(MailItemsPair(ItemID, item->GetMaxStackSize()));
                    ItemCount -= item->GetMaxStackSize();
                }

                _listItemPairs.push_back(MailItemsPair(ItemID, ItemCount));

                if (_listItemPairs.size() > MAX_MAIL_ITEMS)
                {
                    sLog->outError("> SendMailItems: _listItemPairs.size() = %u", (uint32)_listItemPairs.size());
                    break;
                }
            }

            // from console show not existed sender
            MailSender sender(MAIL_NORMAL, player->GetGUIDLow(), MAIL_STATIONERY_DEFAULT);

            SQLTransaction trans = CharacterDatabase.BeginTransaction();

            // fill mail
            MailDraft draft(Subject, Text);

            for (auto itr : _listItemPairs)
            {
                if (Item* item = Item::CreateItem(itr.first, itr.second, player))
                {
                    item->SaveToDB(trans);                               // save for prevent lost at next mail load, if send fail then item will deleted
                    draft.AddItem(item);
                }
            }

            if (Money)
                draft.AddMoney(Money);

            draft.SendMailTo(trans, player, sender);
            CharacterDatabase.CommitTransaction(trans);
        };

        while (DiffTime < PlayedTimeSec)
        {
            if (LastReward < DiffTime)
            {
                ChatHandler handler(player->GetSession());
                std::string Subject, Text, SelfMessage;
                std::string PlayedTimeSecStr = secsToTimeString(DiffTime);

                MailItemsVector ListItemPairs;
                ListItemPairs.push_back(MailItemsPair(CONF_GET_INT("OnlineReward.PerTime.ItemID"), CONF_GET_INT("OnlineReward.PerTime.ItemCount")));

                Subject = warhead::StringFormat("Reward for online %s", PlayedTimeSecStr.c_str());
                Text = warhead::StringFormat("Привет, %s!\nВы играете на нашем сервере уже более %s. Пожалуйста примите подарок", player->GetName().c_str(), PlayedTimeSecStr.c_str());
                SelfMessage = warhead::StringFormat("You were rewarded for online (%s). You can get the award at the post office.", PlayedTimeSecStr.c_str());

                handler.PSendSysMessage("%s", SelfMessage.c_str());
                SendMailItems(player, Subject, Text, 0, ListItemPairs);

                _LastRewardTimePerTimeStore[PlayerGuid] = DiffTime;
            }

            DiffTime += CONF_GET_INT("OnlineReward.PerTime.Time");
        }

        SaveRewardDB(player, false);
    }

    void SaveRewardDB(Player* player, bool IsPerOnline)
    {
        uint64 PlayerGuid = player->GetGUIDLow();

        if (IsPerOnline)
        {
            std::ostringstream ssRewarded;
            auto const& itr = _lastrewardtimestore.find(PlayerGuid);
            if (itr != _lastrewardtimestore.end())
            {
                for (uint32 Rewarded : itr->second)
                    ssRewarded << Rewarded << ',';
            }

            std::string StrReward = ssRewarded.str();
            if (!StrReward.empty())
                StrReward.erase(StrReward.end() - 1, StrReward.end());

            if (StrReward.size() > 0)
                CharacterDatabase.PExecute("UPDATE `online_reward_history` SET `Rewarded` = '%s' WHERE `PlayedGuid` = %u", StrReward.c_str(), PlayerGuid);
        }
        else
            if (_LastRewardTimePerTimeStore[PlayerGuid])
                CharacterDatabase.PExecute("UPDATE `online_reward_history` SET `RewardedPerHour` = %u WHERE `PlayedGuid` = %u", _LastRewardTimePerTimeStore[PlayerGuid], PlayerGuid);
    }

    void LoadRewardFromDB(Player* player, bool IsPerOnline)
    {
        uint64 PlayerGuid = player->GetGUIDLow();
        bool IsExistDB = true;

        QueryResult result = CharacterDatabase.PQuery("SELECT * FROM `online_reward_history` WHERE `PlayedGuid` = %u", PlayerGuid);
        if (!result)
            IsExistDB = false;

        if (!IsExistDB)
            CharacterDatabase.PExecute("INSERT INTO `online_reward_history`(`PlayedGuid`) VALUES (%u)", PlayerGuid);

        if (IsPerOnline && IsExistDB)
        {
            std::string ParamRewarded;

            QueryResult result = CharacterDatabase.PQuery("SELECT Rewarded FROM `online_reward_history` WHERE `PlayedGuid` = %u", PlayerGuid);
            if (result)
                ParamRewarded = result->Fetch()->GetString();

            _lastrewardtimestore.erase(PlayerGuid);

            Tokenizer tokens(ParamRewarded, ',');
            for (uint8 i = 0; i < tokens.size(); )
                _lastrewardtimestore[PlayerGuid].push_back(atoi(tokens[i++]));
        }
        else if (!IsPerOnline && IsExistDB)
        {
            uint32 LasRewardPerHour = 0;

            QueryResult result = CharacterDatabase.PQuery("SELECT RewardedPerHour FROM `online_reward_history` WHERE `PlayedGuid` = %u", PlayerGuid);
            if (result)
                LasRewardPerHour = result->Fetch()->GetUInt32();

            _LastRewardTimePerTimeStore.erase(PlayerGuid);
            _LastRewardTimePerTimeStore[PlayerGuid] = LasRewardPerHour;
        }
    }

    bool IsRewardedPlayer(uint32 Guid, uint32 RewardSecond)
    {
        auto const& itr = _lastrewardtimestore.find(Guid);
        if (itr != _lastrewardtimestore.end())
        {
            for (auto const& itr2 : itr->second)
                if (itr2 == RewardSecond)
                    return true;
        }

        return false;
    }

private:
    std::map<uint32, RewardPlayedTime> rewards;
    std::unordered_map<uint32, std::vector<uint32>> _lastrewardtimestore;
    std::unordered_map<uint32, uint32> _LastRewardTimePerTimeStore;

    // For mails
    typedef std::pair<uint32, uint32> MailItemsPair;
    typedef std::vector<MailItemsPair> MailItemsVector;

    IntervalTimer updateTimer;
};

#define sOL OnlineReward::instance()

class OnlineReward_Player : public PlayerScript
{
public:
    OnlineReward_Player() : PlayerScript("OnlineReward_Player") {}

    void OnBeforeUpdate(Player* player, uint32 p_time) override
    {
        if (!CONF_GET_BOOL("OnlineReward.Enable"))
            return;

        UpdateTimer.Update(p_time);

        if (UpdateTimer.Passed())
        {
            sOL->RewardPlayer(player);
            UpdateTimer.Reset();
        }
    }

    void OnLogin(Player* player) override
    {
        if (!CONF_GET_BOOL("OnlineReward.Enable"))
            return;

        sOL->LoadRewardForPlayer(player);
    }

    void OnLogout(Player* player) override
    {
        if (!CONF_GET_BOOL("OnlineReward.Enable"))
            return;

        sOL->SaveRewardForPlayer(player);
    }
private:

    IntervalTimer& UpdateTimer = sOL->GetUpdateTimer();
};

class OnlineReward_World : public WorldScript
{
public:
    OnlineReward_World() : WorldScript("OnlineReward_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddBoolConfig("OnlineReward.Enable");
        sGameConfig->AddBoolConfig("OnlineReward.PerOnline.Enable");
        sGameConfig->AddBoolConfig("OnlineReward.PerTime.Enable");
        sGameConfig->AddIntConfig("OnlineReward.PerTime.Time", 3600);
        sGameConfig->AddIntConfig("OnlineReward.PerTime.ItemID", 47241);
        sGameConfig->AddIntConfig("OnlineReward.PerTime.ItemCount", 1);
        sGameConfig->AddIntConfig("OnlineReward.CheckTime", 60);

        sOL->SetInterval();
    }

    void OnStartup() override
    {
        if (!CONF_GET_BOOL("OnlineReward.Enable"))
            return;

        sOL->LoadDataFromDB();
    }
};

// Group all custom scripts
void AddSC_OnlineReward()
{
    new OnlineReward_Player();
    new OnlineReward_World();
}
