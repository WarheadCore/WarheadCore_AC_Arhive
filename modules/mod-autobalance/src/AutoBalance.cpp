/*
* Copyright (C) 2020 WarheadCore <https://github.com/WarheadCore/WarheadCore>
* Copyright (C) 2018 warheadCore <http://www.warheadcore.org>
* Copyright (C) 2012 CVMagic <http://www.trinitycore.org/f/topic/6551-vas-autobalance/>
* Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* Copyright (C) 1985-2010 KalCorp  <http://vasserver.dyndns.org/>
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

#include "AutoBalance.h"
#include "Log.h"
#include "GameConfig.h"
#include "ScriptMgrMacros.h"
#include "ObjectMgr.h"

AutoBalance* AutoBalance::instance()
{
    static AutoBalance instance;
    return &instance;
}

void AutoBalance::LoadConfig()
{
    // Add config oprions
    sGameConfig->AddBoolConfig("AB.Enable");
    sGameConfig->AddBoolConfig("AB.LevelUseDbValuesWhenExists");
    sGameConfig->AddBoolConfig("AB.LevelEndGameBoost");
    sGameConfig->AddBoolConfig("AB.DungeonScaleDownXP");
    sGameConfig->AddBoolConfig("AB.DungeonsOnly");
    sGameConfig->AddBoolConfig("AB.PlayerChangeNotify");
    sGameConfig->AddBoolConfig("AB.Reward.Enable");

    sGameConfig->AddFloatConfig("AB.InflectionPoint", 0.5f);
    sGameConfig->AddFloatConfig("AB.InflectionPointHeroic", 0.5f);
    sGameConfig->AddFloatConfig("AB.InflectionPointRaid10M", 0.5f);
    sGameConfig->AddFloatConfig("AB.InflectionPointRaid10MHeroic", 0.5f);
    sGameConfig->AddFloatConfig("AB.InflectionPointRaid25M", 0.5f);
    sGameConfig->AddFloatConfig("AB.InflectionPointRaid25MHeroic", 0.5f);
    sGameConfig->AddFloatConfig("AB.InflectionPointRaid", 0.5f);
    sGameConfig->AddFloatConfig("AB.InflectionPointRaidHeroic", 0.5f);
    sGameConfig->AddFloatConfig("AB.BossInflectionMult", 0.5f);
    sGameConfig->AddFloatConfig("AB.MinHPModifier", 0.01f);
    sGameConfig->AddFloatConfig("AB.MinManaModifier", 0.01f);
    sGameConfig->AddFloatConfig("AB.MinDamageModifier", 0.01f);
    sGameConfig->AddFloatConfig("AB.Rate.Global");
    sGameConfig->AddFloatConfig("AB.Rate.Health");
    sGameConfig->AddFloatConfig("AB.Rate.Mana");
    sGameConfig->AddFloatConfig("AB.Rate.Armor");
    sGameConfig->AddFloatConfig("AB.Rate.Damage");

    sGameConfig->AddIntConfig("AB.LevelScaling", 1);
    sGameConfig->AddIntConfig("AB.LevelHigherOffset", 1);
    sGameConfig->AddIntConfig("AB.LevelLowerOffset", 1);
    sGameConfig->AddIntConfig("AB.DebugLevel", 1);
    sGameConfig->AddIntConfig("AB.PlayerCountDifficultyOffset");
    sGameConfig->AddIntConfig("AB.Reward.RaidToken", 49426);
    sGameConfig->AddIntConfig("AB.Reward.DungeonToken", 47241);
    sGameConfig->AddIntConfig("AB.Reward.MinPlayerReward", 1);

    sGameConfig->AddStringConfig("AB.ForcedID.40", "11583,16441,30057,13020,15589,14435,18192,14889,14888,14887,14890,15302,15818,15742,15741,15740,18338");
    sGameConfig->AddStringConfig("AB.ForcedID.25", "22997,21966,21965,21964,21806,21215,21845,19728,12397,17711,18256,18192");
    sGameConfig->AddStringConfig("AB.ForcedID.20");
    sGameConfig->AddStringConfig("AB.ForcedID.10", "15689,15550,16152,17521,17225,16028,29324,31099");
    sGameConfig->AddStringConfig("AB.ForcedID.5", "8317,15203,15204,15205,15305,6109,26801,30508,26799,30495,26803,30497,27859,27249 ");
    sGameConfig->AddStringConfig("AB.ForcedID.2");
    sGameConfig->AddStringConfig("AB.DisabledID");

    // Check config options
    forcedCreatureIds.clear();
    LoadForcedCreatureIdsFromString(CONF_GET_STR("AB.ForcedID.40"), 40);
    LoadForcedCreatureIdsFromString(CONF_GET_STR("AB.ForcedID.25"), 25);
    LoadForcedCreatureIdsFromString(CONF_GET_STR("AB.ForcedID.10"), 10);
    LoadForcedCreatureIdsFromString(CONF_GET_STR("AB.ForcedID.5"), 5);
    LoadForcedCreatureIdsFromString(CONF_GET_STR("AB.ForcedID.2"), 2);
    LoadForcedCreatureIdsFromString(CONF_GET_STR("AB.DisabledID"), 0);
}

uint32 AutoBalance::GetValidDebugLevel()
{
    uint32 debugLevel = CONF_GET_INT("AB.DebugLevel");

    if (debugLevel < 0 || debugLevel > 3)
        return 1;

    return debugLevel;
}

void AutoBalance::LoadForcedCreatureIdsFromString(std::string creatureIds, uint32 forcedPlayerCount) // Used for reading the string from the configuration file to for those creatures who need to be scaled for XX number of players.
{
    std::string delimitedValue;
    std::stringstream creatureIdsStream;

    creatureIdsStream.str(creatureIds);

    while (std::getline(creatureIdsStream, delimitedValue, ',')) // Process each Creature ID in the string, delimited by the comma - ","
    {
        uint32 creatureId = atoi(delimitedValue.c_str());

        if (creatureId >= 0)
            forcedCreatureIds[creatureId] = forcedPlayerCount;
    }
}

int32 AutoBalance::GetForcedNumPlayers(uint32 creatureId)
{
    if (forcedCreatureIds.find(creatureId) == forcedCreatureIds.end()) // Don't want the forcedCreatureIds map to blowup to a massive empty array
        return -1;

    return forcedCreatureIds[creatureId];
}

void AutoBalance::GetAreaLevel(Map* map, uint8 areaid, uint8& min, uint8& max)
{
    LFGDungeonEntry const* dungeon = GetLFGDungeon(map->GetId(), map->GetDifficulty());
    if (!dungeon)
        return;

    if (map->IsDungeon() || map->IsRaid())
    {
        min = dungeon->minlevel;
        max = dungeon->reclevel ? dungeon->reclevel : dungeon->maxlevel;
    }

    if (min || max)
        return;

    AreaTableEntry const* areaEntry = sAreaTableStore.LookupEntry(areaid);
    if (!areaEntry)
        return;

    if (areaEntry->area_level)
    {
        min = areaEntry->area_level;
        max = areaEntry->area_level;
    }
}

AutoBalanceScript* AutoBalanceScript::instance()
{
    static AutoBalanceScript instance;
    return &instance;
}

bool AutoBalanceScript::OnBeforeModifyAttributes(Creature* creature, uint32& instancePlayerCount)
{
    bool ret = true;

    FOR_SCRIPTS_RET(ABModuleScript, itr, end, ret) // return true by default if not scripts
        if (!itr->second->OnBeforeModifyAttributes(creature, instancePlayerCount))
            ret = false; // we change ret value only when scripts return false

    return ret;
}

bool AutoBalanceScript::OnAfterDefaultMultiplier(Creature* creature, float& defaultMultiplier)
{
    bool ret = true;

    FOR_SCRIPTS_RET(ABModuleScript, itr, end, ret) // return true by default if not scripts
        if (!itr->second->OnAfterDefaultMultiplier(creature, defaultMultiplier))
            ret = false; // we change ret value only when scripts return false

    return ret;
}

bool AutoBalanceScript::OnBeforeUpdateStats(Creature* creature, uint32& scaledHealth, uint32& scaledMana, float& damageMultiplier, uint32& newBaseArmor)
{
    bool ret = true;

    FOR_SCRIPTS_RET(ABModuleScript, itr, end, ret)
        if (!itr->second->OnBeforeUpdateStats(creature, scaledHealth, scaledMana, damageMultiplier, newBaseArmor))
            ret = false;

    return ret;
}

ABModuleScript::ABModuleScript(const char* name)
    : ModuleScript(name)
{
    ScriptRegistry<ABModuleScript>::AddScript(this);
}
