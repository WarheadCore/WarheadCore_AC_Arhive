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
#include "StringFormat.h"

struct LevelRewardStruct
{
    uint32 Money;
    uint32 ItemID;
    uint32 ItemCount;
};

class LevelReward
{
public:
    static LevelReward* instance()
    {
        static LevelReward instance;
        return &instance;
    }

    void LoadDataFromDB()
    {
        uint32 msTime = getMSTime();
        rewards.clear();

        LOG_INFO("module", "Load level reward data...");

        //                                                  0      1      2       3
        QueryResult result = WorldDatabase.Query("SELECT Level, Money, ItemID, ItemCount FROM level_reward ORDER BY Level");
        if (!result)
        {
            sLog->outErrorDb("In DB table `level_reward` not data. Loading canceled"); 
            sLog->outString();
            return;
        }

        do
        {
            Field* fields = result->Fetch();

            LevelRewardStruct _levelReward;

            uint32 Level = fields[0].GetUInt32();

            _levelReward.Money = fields[1].GetUInt32();
            _levelReward.ItemID = fields[2].GetUInt32();
            _levelReward.ItemCount = fields[3].GetUInt32();

            // Проверка
            if (Level > CONF_GET_INT("MaxPlayerLevel"))
            {
                sLog->outErrorDb("-> Level (%u) more, than max player level in world (%u). Skip", Level, CONF_GET_INT("MaxPlayerLevel"));
                continue;
            }

            if (_levelReward.ItemID)
            {
                ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(_levelReward.ItemID);
                if (!itemTemplate)
                {
                    sLog->outErrorDb("-> For level (%u) item witch nuber %u not found. Item delete from reward", Level, _levelReward.ItemID);
                    _levelReward.ItemID = 0;
                }
            }

            if (_levelReward.ItemID && !_levelReward.ItemCount)
            {
                sLog->outErrorDb("-> For level (%u) item witch nuber %u adding 0 count - this useless. Set 1", Level, _levelReward.ItemID);
                _levelReward.ItemCount = 1;
            }

            rewards.insert(std::make_pair(Level, _levelReward));

        } while (result->NextRow());

        sLog->outString(">> Loaded %u reward for level in %u ms", static_cast<uint32>(rewards.size()), GetMSTimeDiffToNow(msTime));
        sLog->outString();
    }

    void RewardPlayer(Player* player, uint8 oldLevel)
    {
        uint8 PlayerLevel = player->getLevel();
        int32 Diff = PlayerLevel - oldLevel;

        if (Diff <= 0 || PlayerLevel == 1)
            return;

        while (Diff)
        {
            RewardForLevel(player, PlayerLevel + 1 - Diff);
            Diff--;
        }
    }    

private:
    std::map<uint32, LevelRewardStruct> rewards;

    LevelRewardStruct const* GetLevelReward(uint32 Level)
    {
        auto const& itr = rewards.find(Level);
        if (itr != rewards.end())
            return &itr->second;

        return nullptr;
    }

    void RewardForLevel(Player* player, uint8 Level)
    {
        if (Level > CONF_GET_INT("MaxPlayerLevel"))
            return;

        auto const& levelReward = GetLevelReward(Level);
        if (!levelReward)
            return;

        ChatHandler handler(player->GetSession());
        std::string Subject, Text, SelfMessage;

        typedef std::pair<uint32, uint32> MailItemsPair;
        typedef std::vector<MailItemsPair> MailItemsVector;

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

        MailItemsVector ListItemPairs;
        ListItemPairs.push_back(MailItemsPair(levelReward->ItemID, levelReward->ItemCount));

        Subject = warhead::StringFormat("Reward for level up to %u", Level);
        Text = warhead::StringFormat("You increased level to %u and get a reward!", Level);
        SelfMessage = warhead::StringFormat("You increased level to %u and get a reward!", Level);

        handler.PSendSysMessage("%s", SelfMessage.c_str());
        SendMailItems(player, Subject, Text, levelReward->Money, ListItemPairs);
    }
};

#define sLR LevelReward::instance()

class LevelReward_Player : public PlayerScript
{
public:
    LevelReward_Player() : PlayerScript("LevelReward_Player") { }

    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        if (!CONF_GET_BOOL("LevelReward.Enable"))
            return;

        sLR->RewardPlayer(player, oldLevel);
    }
};

class LevelReward_World : public WorldScript
{
public:
    LevelReward_World() : WorldScript("LevelReward_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddBoolConfig("LevelReward.Enable");
    }

    void OnStartup() override
    {
        if (!CONF_GET_BOOL("LevelReward.Enable"))
            return;

        sLR->LoadDataFromDB();
    }
};

// Group all custom scripts
void AddSC_LevelReward()
{
    new LevelReward_Player();
    new LevelReward_World();
}
