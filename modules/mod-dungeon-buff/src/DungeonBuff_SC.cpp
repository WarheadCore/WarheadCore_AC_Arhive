/*
 * Copyright (C) since 2020 Andrei Guluaev (Winfidonarleyan/Kargatum) https://github.com/Winfidonarleyan
 * Licence MIT https://opensource.org/MIT
 */

#include "Log.h"
#include "ScriptMgr.h"
#include "GameConfig.h"
#include "Chat.h"
#include "Player.h"
#include "Group.h"

class DungeonBuff
{
public:
    static DungeonBuff* instance()
    {
        static DungeonBuff instance;
        return &instance;
    }

    // Config load
    void LoadConfig()
    {
        sGameConfig->AddBoolConfig("DB.Enable");
        sGameConfig->AddBoolConfig("DB.Default.Enable");
        sGameConfig->AddBoolConfig("DB.Heroic.Enable");
        sGameConfig->AddStringConfig("DB.Buffs.Default");
        sGameConfig->AddStringConfig("DB.Buffs.Heroic");

        _IsEnableDefault = CONF_GET_BOOL("DB.Default.Enable");
        _IsEnableHeroic = CONF_GET_BOOL("DB.Heroic.Enable");

        _buffsContainerDefault.clear();
        _buffsContainerHeroic.clear();

        Tokenizer _listDef(CONF_GET_STR("DB.Buffs.Default"), ',');
        for (uint32 i = 0; i < _listDef.size(); i++)
            _buffsContainerDefault[i + 1] = atoi(_listDef[i]);

        Tokenizer _listHeroic(CONF_GET_STR("DB.Buffs.Heroic"), ',');
        for (uint8 i = 0; i < _listHeroic.size(); i++)
            _buffsContainerHeroic[i + 1] = atoi(_listHeroic[i]);
    }

    // Manage system    
    void ApplyBuffs(Player* player)
    {
        ClearBuffs(player);

        uint8 Diff = player->GetMap()->GetDifficulty();

        if ((!Diff && !_IsEnableDefault) || (Diff && !_IsEnableHeroic))
            return;

        uint32 SpellID = GetSpellForDungeon(Diff, GetPlayerCountInGroup(player));
        if (!SpellID)
            return;

        player->CastSpell(player, SpellID, true);
    }

    void ClearBuffs(Player* player)
    {
        if (!IsPlayerBuffed(player))
            return;

        _playerBuffedStore.erase(player->GetGUID());
    }    

private:
    typedef std::unordered_map<uint64, uint32> PlayerBuffedContainer;
    typedef std::unordered_map<uint8, uint32> BuffsContainerDefault;
    typedef std::unordered_map<uint8, uint32> BuffsContainerHeroic;

    PlayerBuffedContainer _playerBuffedStore;
    BuffsContainerDefault _buffsContainerDefault;
    BuffsContainerHeroic _buffsContainerHeroic;

    bool IsPlayerBuffed(Player* player)
    {
        auto const& itr = _playerBuffedStore.find(player->GetGUID());

        if (itr != _playerBuffedStore.end())
            return true;

        return false;
    }

    uint32 GetPlayerCountInGroup(Player* player)
    {
        uint32 count = 1;
        if (Group* group = player->GetGroup())
            count = static_cast<uint32>(group->GetMemberSlots().size());

        return count;
    }

    uint32 GetSpellForDungeon(uint32 diff, uint32 playerCount)
    {
        if (diff)
        {
            for (auto const& itr : _buffsContainerHeroic)
                if (itr.first == playerCount)
                    return itr.second;
        }
        else
        {
            for (auto const& itr : _buffsContainerDefault)
                if (itr.first == playerCount)
                    return itr.second;
        }

        return 0;
    }

    // From config
    bool _IsEnableDefault;
    bool _IsEnableHeroic;
};

#define sDB DungeonBuff::instance()

class DungeonBuff_Player : public PlayerScript
{
public:
    DungeonBuff_Player() : PlayerScript("DungeonBuff_Player") { }

    void OnMapChanged(Player* player) override
    {
        if (!CONF_GET_BOOL("DB.Enable"))
            return;

        if (!player)
            return;        

        sDB->ClearBuffs(player);

        Map* map = player->GetMap();

        if (map->IsRaid() || map->Is25ManRaid())
            return;

        if (player->GetMap()->IsDungeon())
            sDB->ApplyBuffs(player);
    }
};

class DungeonBuff_World : public WorldScript
{
public:
    DungeonBuff_World() : WorldScript("DungeonBuff_World") { }

    void OnAfterConfigLoad(bool /*Reload*/) override
    {
        sDB->LoadConfig();
    }
};

// Group all custom scripts
void AddSC_DungeonBuff()
{
    new DungeonBuff_World();
}
