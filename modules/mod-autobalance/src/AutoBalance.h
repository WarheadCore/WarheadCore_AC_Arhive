/*
 * Copyright (C) since 2020 Andrei Guluaev (Winfidonarleyan/Kargatum) https://github.com/Winfidonarleyan
 * Licence MIT https://opensource.org/MIT
 */

#ifndef _AUTOBALANCE_H_
#define _AUTOBALANCE_H_

#include "Common.h"
#include "ScriptMgr.h"
#include "Creature.h"

class AutoBalance
{
public:
    static AutoBalance* instance();

    void LoadConfig();

    uint32 GetValidDebugLevel();
    int32 GetForcedNumPlayers(uint32 creatureId);

    void LoadForcedCreatureIdsFromString(std::string creatureIds, uint32 forcedPlayerCount);
    void GetAreaLevel(Map* map, uint8 areaid, uint8& min, uint8& max);

private:
    // The map values correspond with the .AutoBalance.XX.Name entries in the configuration file.
    std::map<uint32, uint32> forcedCreatureIds;
};

#define sAB AutoBalance::instance()

// Manages registration, loading, and execution of scripts.
class AutoBalanceScript
{
public:
    static AutoBalanceScript* instance();

    // called at the start of ModifyCreatureAttributes method
    // it can be used to add some condition to skip autobalancing system for example
    bool OnBeforeModifyAttributes(Creature* creature, uint32& instancePlayerCount);

    // called right after default multiplier has been set, you can use it to change
    // current scaling formula based on number of players or just skip modifications
    bool OnAfterDefaultMultiplier(Creature* creature, float& defaultMultiplier);

    // called before change creature values, to tune some values or skip modifications
    bool OnBeforeUpdateStats(Creature* creature, uint32& scaledHealth, uint32& scaledMana, float& damageMultiplier, uint32& newBaseArmor);  
};

#define sABScript AutoBalanceScript::instance()

/*
* Dedicated hooks for Autobalance Module
* Can be used to extend/customize this system
*/
class ABModuleScript : public ModuleScript
{
protected:

    ABModuleScript(const char* name);

public:
    virtual bool OnBeforeModifyAttributes(Creature* /*creature*/, uint32& /*instancePlayerCount*/) { return true; }
    virtual bool OnAfterDefaultMultiplier(Creature* /*creature*/, float& /*defaultMultiplier*/) { return true; }
    virtual bool OnBeforeUpdateStats(Creature* /*creature*/, uint32&/*scaledHealth*/, uint32&/*scaledMana*/, float&/*damageMultiplier*/, uint32&/*newBaseArmor*/) { return true; }
};

template class ScriptRegistry<ABModuleScript>;

#endif /* _AUTOBALANCE_H_ */
