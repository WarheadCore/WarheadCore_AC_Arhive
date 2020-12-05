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
#include "ModuleLocale.h"
#include "GameConfig.h"
#include "Chat.h"
#include "Player.h"
#include "StringFormat.h"
#include "ExternalMail.h"

enum StringLocales : uint8
{
    LEVEL_REWARD_LOCALE_SUBJECT = 1,
    LEVEL_REWARD_LOCALE_TEXT,
    LEVEL_REWARD_LOCALE_MESSAGE,

    LEVEL_REWARD_LOCALE_MAX
};

#define MODULE_NAME "mod-level-reward"

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
            LOG_ERROR("module", "In DB table `level_reward` not data. Loading canceled");
            LOG_ERROR("module", "");
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
                LOG_ERROR("module", "-> Level (%u) more, than max player level in world (%u). Skip", Level, CONF_GET_INT("MaxPlayerLevel"));
                continue;
            }

            if (_levelReward.ItemID)
            {
                ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(_levelReward.ItemID);
                if (!itemTemplate)
                {
                    LOG_ERROR("module", "-> For level (%u) item witch nuber %u not found. Item delete from reward", Level, _levelReward.ItemID);
                    _levelReward.ItemID = 0;
                }
            }

            if (_levelReward.ItemID && !_levelReward.ItemCount)
            {
                LOG_ERROR("module", "-> For level (%u) item witch nuber %u adding 0 count - this useless. Set 1", Level, _levelReward.ItemID);
                _levelReward.ItemCount = 1;
            }

            rewards.insert(std::make_pair(Level, _levelReward));

        } while (result->NextRow());

        LOG_INFO("module", ">> Loaded %u reward for level in %u ms", static_cast<uint32>(rewards.size()), GetMSTimeDiffToNow(msTime));
        LOG_INFO("module", "");
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

        std::string subject = *sModuleLocale->GetModuleString(MODULE_NAME, LEVEL_REWARD_LOCALE_SUBJECT, Level);
        std::string text = *sModuleLocale->GetModuleString(MODULE_NAME, LEVEL_REWARD_LOCALE_TEXT, Level);

        uint8 localeIndex = static_cast<uint8>(player->GetSession()->GetSessionDbLocaleIndex());

        // Send External mail
        sEM->AddMail(player->GetName(), subject, text, levelReward->ItemID, levelReward->ItemCount, CONF_GET_INT("LevelReward.NpcID"));

        sModuleLocale->SendPlayerMessage(player, MODULE_NAME, LEVEL_REWARD_LOCALE_MESSAGE, Level);
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
        sGameConfig->AddOption<bool>("LevelReward.Enable");
        sGameConfig->AddOption<int32>("LevelReward.NpcID", 37688);
    }

    void OnStartup() override
    {
        if (!CONF_GET_BOOL("LevelReward.Enable"))
            return;

        sModuleLocale->CheckStrings(MODULE_NAME, LEVEL_REWARD_LOCALE_MAX);

        sLR->LoadDataFromDB();
    }
};

// Group all custom scripts
void AddSC_LevelReward()
{
    new LevelReward_Player();
    new LevelReward_World();
}
