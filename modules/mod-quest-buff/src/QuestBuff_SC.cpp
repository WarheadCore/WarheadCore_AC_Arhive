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
#include <vector>

struct QuestBuffStruct
{
    uint32 QuestID;
    uint32 SpellID;
    uint32 Rank;
    uint32 Category;
};

class QuestBuff
{
public:
    static QuestBuff* instance()
    {
        static QuestBuff instance;
        return &instance;
    }

    void LoadSystem()
    {
        if (!CONF_GET_BOOL("QuestBuff.Enable"))
            return;

        uint32 msTime = getMSTime();
        _Store.clear();

        QueryResult result = WorldDatabase.Query("SELECT `QuestID`, `SpellID`, `SpellRank`, `Category` FROM quest_buff ORDER BY `Category`, `SpellRank`");
        if (!result)
        {
            LOG_INFO("modules", ">> In DB table `quest_buff` not data. Loading canceled");
            LOG_INFO("modules", "");
            return;
        }

        do
        {
            Field* fields = result->Fetch();

            uint32 QuestID = fields[0].GetUInt32();

            Quest const* quest = sObjectMgr->GetQuestTemplate(QuestID);
            if (!quest)
                continue;

            QuestBuffStruct QB;
            QB.QuestID = QuestID;
            QB.SpellID = fields[1].GetUInt32();
            QB.Rank = fields[2].GetUInt32();
            QB.Category = fields[3].GetUInt32();

            auto spell = sSpellStore.LookupEntry(QB.SpellID);
            if (!spell)
            {
                LOG_ERROR("modules", "-> Spell with number (%u) not found. Skip.", QB.SpellID);
                continue;
            }

            _Store.push_back(QB);

        } while (result->NextRow());

        LOG_INFO("modules", ">> Loaded %u quest buffs in %u ms", (uint32)_Store.size(), GetMSTimeDiffToNow(msTime));
        LOG_INFO("modules", "");
    }

    uint32 GetHighRankByCategory(Player* player, uint32 Cat)
    {
        uint32 HightRank = 0;

        for (auto const& itr : _Store)
        {
            if (itr.Category != Cat)
                continue;

            if (player->GetQuestStatus(itr.QuestID) != QUEST_STATUS_REWARDED)
                continue;

            HightRank++;
        }

        return HightRank;
    }

    uint32 GetHighSpellByCategory(Player* player, uint32 Cat)
    {
        if(!player)
            return 0;

        uint32 HighSpellRank = GetHighRankByCategory(player, Cat);

        for (auto const& itr : _Store)
        {
            if (itr.Category != Cat)
                continue;

            if (player->GetQuestStatus(itr.QuestID) != QUEST_STATUS_REWARDED)
                continue;

            if (itr.Rank != HighSpellRank)
                continue;

            return itr.SpellID;
        }

        return 0;
    }

private:
    std::vector<QuestBuffStruct> _Store;
};

#define sQuestBuff QuestBuff::instance()

class QuestBuff_Player : public PlayerScript
{
public:
    QuestBuff_Player() : PlayerScript("QuestBuff_Player") { }

    void OnLogin(Player* player) override
    {
        if (!CONF_GET_BOOL("QuestBuff.Enable"))
            return;

        for (uint8 i = 1; i < 4; i++)
            if (sQuestBuff->GetHighSpellByCategory(player, i))
                player->CastSpell(player, sQuestBuff->GetHighSpellByCategory(player, i), true);
    }
};

class QuestBuff_World : public WorldScript
{
public:
    QuestBuff_World() : WorldScript("QuestBuff_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddBoolConfig("QuestBuff.Enable");
    }

    void OnStartup() override
    {
        if (!CONF_GET_BOOL("QuestBuff.Enable"))
            return;

        sQuestBuff->LoadSystem();
    }
};

// Group all custom scripts
void AddSC_QuestBuff()
{
    new QuestBuff_Player();
    new QuestBuff_World();
}
