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

#include "AutoBalance.h"
#include "Log.h"
#include "ScriptMgr.h"
#include "GameConfig.h"
#include "Chat.h"
#include "Player.h"
#include "MapManager.h"
#include "Language.h"

class AutoBalanceMapInfo : public DataMap::Base
{
public:
    AutoBalanceMapInfo() { }
    AutoBalanceMapInfo(uint32 count, uint8 selLevel) : playerCount(count), mapLevel(selLevel) {}

    uint32 playerCount = 0;
    uint8 mapLevel = 0;
};

class AutoBalanceCreatureInfo : public DataMap::Base
{
public:
    AutoBalanceCreatureInfo() { }
    AutoBalanceCreatureInfo(uint32 count, float dmg, float hpRate, float manaRate, float armorRate, uint8 selLevel) :
        instancePlayerCount(count), selectedLevel(selLevel), DamageMultiplier(dmg), HealthMultiplier(hpRate), ManaMultiplier(manaRate), ArmorMultiplier(armorRate) {}

    uint32 instancePlayerCount = 0;
    uint8 selectedLevel = 0;

    // this is used to detect creatures that update their entry
    uint32 entry = 0;
    float DamageMultiplier = 1;
    float HealthMultiplier = 1;
    float ManaMultiplier = 1;
    float ArmorMultiplier = 1;
};

class AutoBalance_World : public WorldScript
{
public:
    AutoBalance_World() : WorldScript("AutoBalance_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sAB->LoadConfig();
    }
};

class AutoBalance_Player : public PlayerScript
{
public:
    AutoBalance_Player() : PlayerScript("AutoBalance_Player") { }

    void OnLevelChanged(Player* player, uint8 /*oldlevel*/) override
    {
        if (!CONF_GET_BOOL("AB.Enable") || !player)
            return;

        if (CONF_GET_INT("AB.LevelScaling") == 0)
            return;

        AutoBalanceMapInfo* mapABInfo = player->GetMap()->CustomData.GetDefault<AutoBalanceMapInfo>("AutoBalanceMapInfo");

        if (mapABInfo->mapLevel < player->getLevel())
            mapABInfo->mapLevel = player->getLevel();
    }

    void OnGiveXP(Player* player, uint32& amount, Unit* victim) override
    {
        if (!victim || !CONF_GET_BOOL("AB.DungeonScaleDownXP"))
            return;

        Map* map = player->GetMap();
        if (!map->IsDungeon())
            return;

        // Ensure that the players always get the same XP, even when entering the dungeon alone
        uint32 maxPlayerCount = ((InstanceMap*)sMapMgr->FindMap(map->GetId(), map->GetInstanceId()))->GetMaxPlayers();
        uint32 currentPlayerCount = map->GetPlayersCountExceptGMs();
        amount *= (float)currentPlayerCount / maxPlayerCount;
    }
};

class AutoBalance_Unit : public UnitScript
{
public:
    AutoBalance_Unit() : UnitScript("AutoBalance_Unit", true) { }

    uint32 DealDamage(Unit* AttackerUnit, Unit* playerVictim, uint32 damage, DamageEffectType /*damagetype*/) override
    {
        return _Modifer_DealDamage(playerVictim, AttackerUnit, damage);
    }

    void ModifyPeriodicDamageAurasTick(Unit* target, Unit* attacker, uint32& damage) override
    {
        damage = _Modifer_DealDamage(target, attacker, damage);
    }

    void ModifySpellDamageTaken(Unit* target, Unit* attacker, int32& damage) override
    {
        damage = _Modifer_DealDamage(target, attacker, damage);
    }

    void ModifyMeleeDamage(Unit* target, Unit* attacker, uint32& damage) override
    {
        damage = _Modifer_DealDamage(target, attacker, damage);
    }

    void ModifyHealRecieved(Unit* target, Unit* attacker, uint32& damage) override
    {
        damage = _Modifer_DealDamage(target, attacker, damage);
    }

private:
    uint32 _Modifer_DealDamage(Unit* target, Unit* attacker, uint32 damage)
    {
        if (!CONF_GET_BOOL("AB.Enable"))
            return damage;

        if (!attacker || attacker->GetTypeId() == TYPEID_PLAYER || !attacker->IsInWorld())
            return damage;

        float damageMultiplier = attacker->CustomData.GetDefault<AutoBalanceCreatureInfo>("AutoBalanceCreatureInfo")->DamageMultiplier;

        if (damageMultiplier == 1)
            return damage;

        if (!(!CONF_GET_BOOL("AB.DungeonsOnly") || (target->GetMap()->IsDungeon() && attacker->GetMap()->IsDungeon()) || (attacker->GetMap()->IsBattleground()
                && target->GetMap()->IsBattleground())))
            return damage;

        if ((attacker->IsHunterPet() || attacker->IsPet() || attacker->IsSummon()) && attacker->IsControlledByPlayer())
            return damage;

        return damage * damageMultiplier;
    }
};

class AutoBalance_AllMap : public AllMapScript
{
public:
    AutoBalance_AllMap() : AllMapScript("AutoBalance_AllMap") { }

    void OnPlayerEnterAll(Map* map, Player* player) override
    {
        if (!CONF_GET_BOOL("AB.Enable"))
            return;

        if (player->IsGameMaster())
            return;

        AutoBalanceMapInfo* mapABInfo = map->CustomData.GetDefault<AutoBalanceMapInfo>("AutoBalanceMapInfo");

        // always check level, even if not conf CONF_GET_BOOL("AB.Enable")
        // because we can enable at runtime and we need this information
        if (player && (player->getLevel() > mapABInfo->mapLevel))
            mapABInfo->mapLevel = player->getLevel();
        else
        {
            auto const& playerList = map->GetPlayers();
            if (!playerList.isEmpty())
            {
                for (auto const& itr : playerList)
                {
                    if (Player* playerHandle = itr.GetSource())
                    {
                        if (!playerHandle->IsGameMaster() && playerHandle->getLevel() > mapABInfo->mapLevel)
                            mapABInfo->mapLevel = playerHandle->getLevel();
                    }
                }
            }
        }

        mapABInfo->playerCount++;

        if (!CONF_GET_BOOL("AB.PlayerChangeNotify"))
            return;

        if (!player || !map->GetEntry()->IsDungeon())
            return;

        auto const& playerList = map->GetPlayers();
        if (!playerList.isEmpty())
        {
            for (auto const& itr : playerList)
            {
                if (Player* playerHandle = itr.GetSource())
                {
                    ChatHandler chatHandle = ChatHandler(playerHandle->GetSession());
                    chatHandle.PSendSysMessage("|cffFF0000[AutoBalance]|r|cffFF8000 %s entered the Instance %s. Auto setting player count to %u (Player Difficulty Offset = %u) |r",
                        player->GetName().c_str(), map->GetMapName(), mapABInfo->playerCount + CONF_GET_INT("AB.PlayerCountDifficultyOffset"), CONF_GET_INT("AB.PlayerCountDifficultyOffset"));
                }
            }
        }
    }

    void OnPlayerLeaveAll(Map* map, Player* player) override
    {
        if (!CONF_GET_BOOL("AB.Enable"))
            return;

        if (player->IsGameMaster())
            return;

        AutoBalanceMapInfo* mapABInfo = map->CustomData.GetDefault<AutoBalanceMapInfo>("AutoBalanceMapInfo");

        mapABInfo->playerCount--;// (maybe we've to found a safe solution to avoid player recount each time)

        // always check level, even if not conf CONF_GET_BOOL("AB.Enable")
        // because we can enable at runtime and we need this information
        if (!mapABInfo->playerCount)
        {
            mapABInfo->mapLevel = 0;
            return;
        }

        if (!CONF_GET_BOOL("AB.PlayerChangeNotify"))
            return;

        if (!player || !map->GetEntry()->IsDungeon())
            return;

        auto const& playerList = map->GetPlayers();
        if (!playerList.isEmpty())
        {
            for (auto const& itr : playerList)
            {
                if (Player* playerHandle = itr.GetSource())
                {
                    ChatHandler chatHandle = ChatHandler(playerHandle->GetSession());
                    chatHandle.PSendSysMessage("|cffFF0000 [-AutoBalance]|r|cffFF8000 %s left the Instance %s. Auto setting player count to %u (Player Difficulty Offset = %u) |r",
                        player->GetName().c_str(), map->GetMapName(), mapABInfo->playerCount, CONF_GET_INT("AB.PlayerCountDifficultyOffset"));
                }
            }
        }
    }
};

class AutoBalance_AllCreature : public AllCreatureScript
{
public:
    AutoBalance_AllCreature() : AllCreatureScript("AutoBalance_AllCreature") {  }

    void Creature_SelectLevel(const CreatureTemplate* /*creatureTemplate*/, Creature* creature) override
    {
        if (!CONF_GET_BOOL("AB.Enable"))
            return;

        ModifyCreatureAttributes(creature, true);
    }

    void OnAllCreatureUpdate(Creature* creature, uint32 /*diff*/) override
    {
        if (!CONF_GET_BOOL("AB.Enable"))
            return;

        ModifyCreatureAttributes(creature);
    }

private:
    bool checkLevelOffset(uint8 selectedLevel, uint8 targetLevel)
    {
        return selectedLevel && ((targetLevel >= selectedLevel && targetLevel <= (selectedLevel + CONF_GET_INT("AB.LevelHigherOffset"))) || (targetLevel <= selectedLevel && targetLevel >= (selectedLevel - CONF_GET_INT("AB.LevelLowerOffset"))));
    }

    void ModifyCreatureAttributes(Creature* creature, bool resetSelLevel = false)
    {
        if (!creature || !creature->GetMap())
            return;

        if (!creature->GetMap()->IsDungeon() && !creature->GetMap()->IsBattleground() && CONF_GET_BOOL("AB.DungeonsOnly"))
            return;

        if (((creature->IsHunterPet() || creature->IsPet() || creature->IsSummon()) && creature->IsControlledByPlayer()))
            return;

        AutoBalanceMapInfo* mapABInfo = creature->GetMap()->CustomData.GetDefault<AutoBalanceMapInfo>("AutoBalanceMapInfo");
        if (!mapABInfo->mapLevel)
            return;

        CreatureTemplate const* creatureTemplate = creature->GetCreatureTemplate();

        InstanceMap* instanceMap = ((InstanceMap*)sMapMgr->FindMap(creature->GetMapId(), creature->GetInstanceId()));
        uint32 maxNumberOfPlayers = instanceMap->GetMaxPlayers();
        int forcedNumPlayers = sAB->GetForcedNumPlayers(creatureTemplate->Entry);

        if (forcedNumPlayers > 0)
            maxNumberOfPlayers = forcedNumPlayers; // Force maxNumberOfPlayers to be changed to match the Configuration entries ForcedID2, ForcedID5, ForcedID10, ForcedID20, ForcedID25, ForcedID40
        else if (forcedNumPlayers == 0)
            return; // forcedNumPlayers 0 means that the creature is contained in DisabledID -> no scaling

        AutoBalanceCreatureInfo* creatureABInfo = creature->CustomData.GetDefault<AutoBalanceCreatureInfo>("AutoBalanceCreatureInfo");

        // force resetting selected level.
        // this is also a "workaround" to fix bug of not recalculated
        // attributes when UpdateEntry has been used.
        // TODO: It's better and faster to implement a core hook
        // in that position and force a recalculation then
        if ((creatureABInfo->entry != 0 && creatureABInfo->entry != creature->GetEntry()) || resetSelLevel)
            creatureABInfo->selectedLevel = 0; // force a recalculation

        if (!creature->IsAlive())
            return;

        uint32 curCount = mapABInfo->playerCount + CONF_GET_INT("AB.PlayerCountDifficultyOffset");
        uint8 bonusLevel = creatureTemplate->rank == CREATURE_ELITE_WORLDBOSS ? 3 : 0;

        // already scaled
        if (creatureABInfo->selectedLevel)
        {
            if (CONF_GET_INT("AB.LevelScaling"))
            {
                if (checkLevelOffset(mapABInfo->mapLevel + bonusLevel, creature->getLevel()) &&
                    checkLevelOffset(creatureABInfo->selectedLevel, creature->getLevel()) &&
                    creatureABInfo->instancePlayerCount == curCount)
                    return;
            }
            else if (creatureABInfo->instancePlayerCount == curCount)
                return;
        }

        creatureABInfo->instancePlayerCount = curCount;

        if (!creatureABInfo->instancePlayerCount) // no players in map, do not modify attributes
            return;

        if (!sABScript->OnBeforeModifyAttributes(creature, creatureABInfo->instancePlayerCount))
            return;

        uint8 originalLevel = creatureTemplate->maxlevel;
        uint8 level = mapABInfo->mapLevel;
        uint8 areaMinLvl, areaMaxLvl;

        sAB->GetAreaLevel(creature->GetMap(), creature->GetAreaId(), areaMinLvl, areaMaxLvl);

        // avoid level changing for critters and special creatures (spell summons etc.) in instances
        bool skipLevel = false;
        if (originalLevel <= 1 && areaMinLvl >= 5)
            skipLevel = true;

        // change level only whithin the offsets and when in dungeon/raid
        if (CONF_GET_INT("AB.LevelScaling") && creature->GetMap()->IsDungeon() && !skipLevel && !checkLevelOffset(level, originalLevel))
        {  
            if (level != creatureABInfo->selectedLevel || creatureABInfo->selectedLevel != creature->getLevel())
            {
                // keep bosses +3 level
                creatureABInfo->selectedLevel = level + bonusLevel;
                creature->SetLevel(creatureABInfo->selectedLevel);
            }
        }
        else
            creatureABInfo->selectedLevel = creature->getLevel();

        creatureABInfo->entry = creature->GetEntry();

        bool useDefStats = false;
        if (CONF_GET_BOOL("AB.LevelUseDbValuesWhenExists") && creature->getLevel() >= creatureTemplate->minlevel && creature->getLevel() <= creatureTemplate->maxlevel)
            useDefStats = true;

        CreatureBaseStats const* origCreatureStats = sObjectMgr->GetCreatureBaseStats(originalLevel, creatureTemplate->unit_class);
        CreatureBaseStats const* creatureStats = sObjectMgr->GetCreatureBaseStats(creatureABInfo->selectedLevel, creatureTemplate->unit_class);

        uint32 baseHealth = origCreatureStats->GenerateHealth(creatureTemplate);
        uint32 baseMana = origCreatureStats->GenerateMana(creatureTemplate);
        uint32 scaledHealth = 0;
        uint32 scaledMana = 0;

        // Note: CONF_GET_FLOAT("AB.InflectionPoint") handle the number of players required to get 50% health.
        //       you'd adjust this to raise or lower the hp modifier for per additional player in a non-whole group.
        //
        //       diff modify the rate of percentage increase between
        //       number of players. Generally the closer to the value of 1 you have this
        //       the less gradual the rate will be. For example in a 5 man it would take 3
        //       total players to face a mob at full health.
        //
        //       The +1 and /2 values raise the TanH function to a positive range and make
        //       sure the modifier never goes above the value or 1.0 or below 0.
        //

        float defaultMultiplier = 1.0f;

        if (creatureABInfo->instancePlayerCount < maxNumberOfPlayers)
        {
            float inflectionValue = (float)maxNumberOfPlayers;

            if (instanceMap->IsHeroic())
            {
                if (instanceMap->IsRaid())
                {
                    switch (instanceMap->GetMaxPlayers())
                    {
                    case 10:
                        inflectionValue *= CONF_GET_FLOAT("AB.InflectionPointRaid10MHeroic");
                        break;
                    case 25:
                        inflectionValue *= CONF_GET_FLOAT("AB.InflectionPointRaid25MHeroic");
                        break;
                    default:
                        inflectionValue *= CONF_GET_FLOAT("AB.InflectionPointRaidHeroic");
                    }
                }
                else
                    inflectionValue *= CONF_GET_FLOAT("AB.InflectionPointHeroic");
            }
            else
            {
                if (instanceMap->IsRaid())
                {
                    switch (instanceMap->GetMaxPlayers())
                    {
                    case 10:
                        inflectionValue *= CONF_GET_FLOAT("AB.InflectionPointRaid10M");
                        break;
                    case 25:
                        inflectionValue *= CONF_GET_FLOAT("AB.InflectionPointRaid25M");
                        break;
                    default:
                        inflectionValue *= CONF_GET_FLOAT("AB.InflectionPointRaid");
                    }
                }
                else
                    inflectionValue *= CONF_GET_FLOAT("AB.InflectionPoint");
            }
            if (creature->IsDungeonBoss())
                inflectionValue *= CONF_GET_FLOAT("AB.BossInflectionMult");

            float diff = ((float)maxNumberOfPlayers / 5) * 1.5f;
            defaultMultiplier = (tanh(((float)creatureABInfo->instancePlayerCount - inflectionValue) / diff) + 1.0f) / 2.0f;
        }

        if (!sABScript->OnAfterDefaultMultiplier(creature, defaultMultiplier))
            return;

        creatureABInfo->HealthMultiplier = CONF_GET_FLOAT("AB.Rate.Health") * defaultMultiplier * CONF_GET_FLOAT("AB.Rate.Global");

        if (creatureABInfo->HealthMultiplier <= CONF_GET_FLOAT("AB.MinHPModifier"))
            creatureABInfo->HealthMultiplier = CONF_GET_FLOAT("AB.MinHPModifier");

        float hpStatsRate = 1.0f;

        if (!useDefStats && CONF_GET_INT("AB.LevelScaling") && !skipLevel)
        {
            float newBaseHealth = 0;

            if (level <= 60)
                newBaseHealth = creatureStats->BaseHealth[0];
            else if (level <= 70)
                newBaseHealth = creatureStats->BaseHealth[1];
            else
            {
                newBaseHealth = creatureStats->BaseHealth[2];

                // special increasing for end-game contents
                if (CONF_GET_BOOL("AB.LevelEndGameBoost"))
                    newBaseHealth *= creatureABInfo->selectedLevel >= 75 && originalLevel < 75 ? float(creatureABInfo->selectedLevel - 70) * 0.3f : 1;
            }

            float newHealth = newBaseHealth * creatureTemplate->ModHealth;

            // allows health to be different with creatures that originally
            // differentiate their health by different level instead of multiplier field.
            // expecially in dungeons. The health reduction decrease if original level is similar to the area max level
            if (originalLevel >= areaMinLvl && originalLevel < areaMaxLvl)
            {
                float reduction = newHealth / float(areaMaxLvl - areaMinLvl) * (float(areaMaxLvl - originalLevel) * 0.3f); // never more than 30%

                if (reduction > 0 && reduction < newHealth)
                    newHealth -= reduction;
            }

            hpStatsRate = newHealth / float(baseHealth);
        }

        creatureABInfo->HealthMultiplier *= hpStatsRate;

        scaledHealth = round(((float)baseHealth * creatureABInfo->HealthMultiplier) + 1.0f);

        //Getting the list of Classes in this group - this will be used later on to determine what additional scaling will be required based on the ratio of tank/dps/healer
        //GetPlayerClassList(creature, playerClassList); // Update playerClassList with the list of all the participating Classes

        float manaStatsRate = 1.0f;
        if (!useDefStats && CONF_GET_INT("AB.LevelScaling") && !skipLevel)
        {
            float newMana = creatureStats->GenerateMana(creatureTemplate);
            manaStatsRate = newMana / float(baseMana);
        }

        creatureABInfo->ManaMultiplier = manaStatsRate * CONF_GET_FLOAT("AB.Rate.Mana") * defaultMultiplier * CONF_GET_FLOAT("AB.Rate.Global");

        if (creatureABInfo->ManaMultiplier <= CONF_GET_FLOAT("AB.MinManaModifier"))
            creatureABInfo->ManaMultiplier = CONF_GET_FLOAT("AB.MinManaModifier");

        scaledMana = round(baseMana * creatureABInfo->ManaMultiplier);

        float damageMul = defaultMultiplier * CONF_GET_FLOAT("AB.Rate.Global") * CONF_GET_FLOAT("AB.Rate.Damage");

        // Can not be less then Min_D_Mod
        if (damageMul <= CONF_GET_FLOAT("AB.MinDamageModifier"))
            damageMul = CONF_GET_FLOAT("AB.MinDamageModifier");

        if (!useDefStats && CONF_GET_INT("AB.LevelScaling") && !skipLevel)
        {
            float origDmgBase = origCreatureStats->GenerateBaseDamage(creatureTemplate);
            float newDmgBase = 0;

            if (level <= 60)
                newDmgBase = creatureStats->BaseDamage[0];
            else if (level <= 70)
                newDmgBase = creatureStats->BaseDamage[1];
            else
            {
                newDmgBase = creatureStats->BaseDamage[2];

                // special increasing for end-game contents
                if (CONF_GET_BOOL("AB.LevelEndGameBoost") && !creature->GetMap()->IsRaid())
                    newDmgBase *= creatureABInfo->selectedLevel >= 75 && originalLevel < 75 ? float(creatureABInfo->selectedLevel - 70) * 0.3f : 1;
            }

            damageMul *= newDmgBase / origDmgBase;
        }

        creatureABInfo->ArmorMultiplier = CONF_GET_FLOAT("AB.Rate.Global") * CONF_GET_FLOAT("AB.Rate.Armor");
        uint32 newBaseArmor = round(creatureABInfo->ArmorMultiplier * (useDefStats || !CONF_GET_INT("AB.LevelScaling") || skipLevel ? origCreatureStats->GenerateArmor(creatureTemplate) : creatureStats->GenerateArmor(creatureTemplate)));

        if (!sABScript->OnBeforeUpdateStats(creature, scaledHealth, scaledMana, damageMul, newBaseArmor))
            return;

        uint32 prevMaxHealth = creature->GetMaxHealth();
        uint32 prevMaxPower = creature->GetMaxPower(POWER_MANA);
        uint32 prevHealth = creature->GetHealth();
        uint32 prevPower = creature->GetPower(POWER_MANA);

        Powers pType = creature->getPowerType();

        creature->SetArmor(newBaseArmor);
        creature->SetModifierValue(UNIT_MOD_ARMOR, BASE_VALUE, (float)newBaseArmor);
        creature->SetCreateHealth(scaledHealth);
        creature->SetMaxHealth(scaledHealth);
        creature->ResetPlayerDamageReq();
        creature->SetCreateMana(scaledMana);
        creature->SetMaxPower(POWER_MANA, scaledMana);
        creature->SetModifierValue(UNIT_MOD_ENERGY, BASE_VALUE, (float)100.0f);
        creature->SetModifierValue(UNIT_MOD_RAGE, BASE_VALUE, (float)100.0f);
        creature->SetModifierValue(UNIT_MOD_HEALTH, BASE_VALUE, (float)scaledHealth);
        creature->SetModifierValue(UNIT_MOD_MANA, BASE_VALUE, (float)scaledMana);

        creatureABInfo->DamageMultiplier = damageMul;

        uint32 scaledCurHealth = prevHealth && prevMaxHealth ? float(scaledHealth) / float(prevMaxHealth) * float(prevHealth) : 0;
        uint32 scaledCurPower = prevPower && prevMaxPower ? float(scaledMana) / float(prevMaxPower) * float(prevPower) : 0;

        creature->SetHealth(scaledCurHealth);

        if (pType == POWER_MANA)
            creature->SetPower(POWER_MANA, scaledCurPower);
        else
            creature->setPowerType(pType); // fix creatures with different power types

        creature->UpdateAllStats();
    }
};

class AutoBalance_Command : public CommandScript
{
public:
    AutoBalance_Command() : CommandScript("AutoBalance_Command") { }

    std::vector<ChatCommand> GetCommands() const
    {
        static std::vector<ChatCommand> ABCommandTable =
        {
            { "setoffset",      SEC_GAMEMASTER,     true, &HandleABSetOffsetCommand,        "Sets the global Player Difficulty Offset for instances. Example: (You + offset(1) = 2 player difficulty)." },
            { "getoffset",      SEC_GAMEMASTER,     true, &HandleABGetOffsetCommand,        "Shows current global player offset value" },
            { "checkmap",       SEC_GAMEMASTER,     true, &HandleABCheckMapCommand,         "Run a check for current map/instance, it can help in case you're testing autobalance with GM." },
            { "mapstat",        SEC_GAMEMASTER,     true, &HandleABMapStatsCommand,         "Shows current autobalance information for this map-" },
            { "creaturestat",   SEC_GAMEMASTER,     true, &HandleABCreatureStatsCommand,    "Shows current autobalance information for selected creature." },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "autobalance",    SEC_GAMEMASTER,     false,      nullptr,    "",     ABCommandTable },
        };

        return commandTable;
    }

    static bool HandleABSetOffsetCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
        {
            handler->PSendSysMessage(".autobalance setoffset #");
            handler->PSendSysMessage("Sets the Player Difficulty Offset for instances. Example: (You + offset(1) = 2 player difficulty).");
            return false;
        }

        char* offset = strtok((char*)args, " ");
        int32 offseti = -1;

        if (offset)
        {
            offseti = static_cast<uint32>(atoi(offset));
            handler->PSendSysMessage("Changing Player Difficulty Offset to %i.", offseti);
            sGameConfig->SetInt("AB.PlayerCountDifficultyOffset", offseti);
            return true;
        }
        else
            handler->PSendSysMessage("Error changing Player Difficulty Offset! Please try again.");

        return false;
    }

    static bool HandleABGetOffsetCommand(ChatHandler* handler, const char* /*args*/)
    {
        handler->PSendSysMessage("Current Player Difficulty Offset = %i", CONF_GET_INT("AB.PlayerCountDifficultyOffset"));
        return true;
    }

    static bool HandleABCheckMapCommand(ChatHandler* handler, const char* args)
    {
        Player* pl = handler->getSelectedPlayer();

        if (!pl)
        {
            handler->SendSysMessage(LANG_SELECT_PLAYER_OR_PET);
            handler->SetSentErrorMessage(true);
            return false;
        }

        AutoBalanceMapInfo* mapABInfo = pl->GetMap()->CustomData.GetDefault<AutoBalanceMapInfo>("AutoBalanceMapInfo");

        mapABInfo->playerCount = pl->GetMap()->GetPlayersCountExceptGMs();

        uint8 level = 0;

        auto const& playerList = pl->GetMap()->GetPlayers();
        if (!playerList.isEmpty())
        {
            for (auto const& itr : playerList)
            {
                if (Player* playerHandle = itr.GetSource())
                {
                    if (playerHandle->getLevel() > level)
                        mapABInfo->mapLevel = level = playerHandle->getLevel();
                }
            }
        }

        HandleABMapStatsCommand(handler, args);

        return true;
    }

    static bool HandleABMapStatsCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* pl = handler->getSelectedPlayer();

        if (!pl)
        {
            handler->SendSysMessage(LANG_SELECT_PLAYER_OR_PET);
            handler->SetSentErrorMessage(true);
            return false;
        }

        AutoBalanceMapInfo* mapABInfo = pl->GetMap()->CustomData.GetDefault<AutoBalanceMapInfo>("AutoBalanceMapInfo");

        handler->PSendSysMessage("Players on map: %u", mapABInfo->playerCount);
        handler->PSendSysMessage("Max level of players in this map: %u", mapABInfo->mapLevel);

        return true;
    }

    static bool HandleABCreatureStatsCommand(ChatHandler* handler, const char* /*args*/)
    {
        Creature* target = handler->getSelectedCreature();

        if (!target)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        AutoBalanceCreatureInfo* creatureABInfo = target->CustomData.GetDefault<AutoBalanceCreatureInfo>("AutoBalanceCreatureInfo");

        handler->PSendSysMessage("Instance player Count: %u", creatureABInfo->instancePlayerCount);
        handler->PSendSysMessage("Selected level: %u", creatureABInfo->selectedLevel);
        handler->PSendSysMessage("Damage multiplier: %.6f", creatureABInfo->DamageMultiplier);
        handler->PSendSysMessage("Health multiplier: %.6f", creatureABInfo->HealthMultiplier);
        handler->PSendSysMessage("Mana multiplier: %.6f", creatureABInfo->ManaMultiplier);
        handler->PSendSysMessage("Armor multiplier: %.6f", creatureABInfo->ArmorMultiplier);

        return true;
    }
};

class AutoBalance_Global : public GlobalScript
{
public:
    AutoBalance_Global() : GlobalScript("AutoBalance_Global") { }

    void OnAfterUpdateEncounterState(Map* map, EncounterCreditType type, uint32 /*creditEntry*/, Unit* source, Difficulty /*difficulty_fixed*/, DungeonEncounterList const* /*encounters*/, uint32 /*dungeonCompleted*/, bool updated) override {

        if (!CONF_GET_BOOL("AB.Reward.Enable") || !updated)
            return;

        if (map->GetPlayersCountExceptGMs() < CONF_GET_INT("AB.Reward.MinPlayerReward"))
            return;

        AutoBalanceMapInfo* mapABInfo = map->CustomData.GetDefault<AutoBalanceMapInfo>("AutoBalanceMapInfo");

        uint8 areaMinLvl, areaMaxLvl;
        sAB->GetAreaLevel(map, source->GetAreaId(), areaMinLvl, areaMaxLvl);

        // skip if it's not a pre-wotlk dungeon/raid and if it's not scaled
        if (!CONF_GET_INT("AB.LevelScaling") || CONF_GET_INT("AB.LevelLowerOffset") >= 10 || mapABInfo->mapLevel <= 70 || areaMinLvl > 70
            // skip when not in dungeon or not kill credit
            || type != ENCOUNTER_CREDIT_KILL_CREATURE || !map->IsDungeon())
            return;

        auto const& playerList = map->GetPlayers();

        if (playerList.isEmpty())
            return;

        uint32 reward = map->IsRaid() ? CONF_GET_INT("AB.Reward.RaidToken") : CONF_GET_INT("AB.Reward.DungeonToken");
        if (!reward)
            return;

        //instanceStart=0, endTime;
        uint8 difficulty = map->GetDifficulty();

        for (auto const& itr : playerList)
        {
            if (!itr.GetSource() || itr.GetSource()->IsGameMaster() || itr.GetSource()->getLevel() < DEFAULT_MAX_LEVEL)
                continue;

            itr.GetSource()->AddItem(reward, 1 + difficulty); // difficulty boost
        }
    }
};

// Group all custom scripts
void AddSC_AutoBalance()
{
    new AutoBalance_World();
    new AutoBalance_Player();
    new AutoBalance_Unit();
    new AutoBalance_AllCreature();
    new AutoBalance_AllMap();
    new AutoBalance_Command();
    new AutoBalance_Global();
}
