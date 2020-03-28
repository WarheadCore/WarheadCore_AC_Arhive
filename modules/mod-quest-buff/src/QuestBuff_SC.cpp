/*
 * Copyright (C) since 2020 Andrei Guluaev (Winfidonarleyan/Kargatum) https://github.com/Winfidonarleyan
 * Licence MIT https://opensource.org/MIT
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
            sLog->outString(">> In DB table `quest_buff` not data. Loading canceled");
            sLog->outString();
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
                sLog->outError("-> Spell with number (%u) not found. Skip.", QB.SpellID);
                continue;
            }

            _Store.push_back(QB);

        } while (result->NextRow());

        sLog->outString(">> Loaded %u quest buffs in %u ms", (uint32)_Store.size(), GetMSTimeDiffToNow(msTime));
        sLog->outString();
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
