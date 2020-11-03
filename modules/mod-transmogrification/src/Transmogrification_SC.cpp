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

#include "Transmogrification.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "GameConfig.h"

class Transmogrification_NPC : public CreatureScript
{
public:
    Transmogrification_NPC() : CreatureScript("Transmogrification_NPC") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        sTransmog->OnGossipHello(player, creature);
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        sTransmog->OnGossipSelect(player, creature, action, sender);
        return true;
    }

    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, const char* code) override
    {
        sTransmog->OnGossipSelectCode(player, creature, action, sender, code);
        return true;
    }
};

class Transmogrification_Player : public PlayerScript
{
public:
    Transmogrification_Player() : PlayerScript("Player_Transmogrify") { }

    void OnAfterSetVisibleItemSlot(Player* player, uint8 slot, Item* item) override
    {
        sTransmog->SetVisibleItemSlot(player, slot, item);
    }

    void OnAfterMoveItemFromInventory(Player* /*player*/, Item* item, uint8 /*bag*/, uint8 /*slot*/, bool /*update*/) override
    {
        sTransmog->DeleteFakeFromDB(item->GetGUIDLow());
    }

    void OnLogin(Player* player) override
    {
        sTransmog->LoadPlayerAtLogin(player);
    }

    void OnLogout(Player* player) override
    {
        sTransmog->ClearPlayerAtLogout(player);
    }
};

class Transmogrification_World : public WorldScript
{
public:
    Transmogrification_World() : WorldScript("Transmogrification_World") { }

    void OnAfterConfigLoad(bool reload) override
    {
        // Add bool
        sGameConfig->AddBoolConfig("Transmogrification.EnableSetInfo");
        sGameConfig->AddBoolConfig("Transmogrification.EnableSets", true);
        sGameConfig->AddBoolConfig("Transmogrification.EnableTransmogInfo", true);
        sGameConfig->AddBoolConfig("Transmogrification.RequireToken");
        sGameConfig->AddBoolConfig("Transmogrification.AllowPoor");
        sGameConfig->AddBoolConfig("Transmogrification.AllowCommon");
        sGameConfig->AddBoolConfig("Transmogrification.AllowUncommon", true);
        sGameConfig->AddBoolConfig("Transmogrification.AllowRare", true);
        sGameConfig->AddBoolConfig("Transmogrification.AllowEpic", true);
        sGameConfig->AddBoolConfig("Transmogrification.AllowLegendary");
        sGameConfig->AddBoolConfig("Transmogrification.AllowArtifact");
        sGameConfig->AddBoolConfig("Transmogrification.AllowHeirloom", true);
        sGameConfig->AddBoolConfig("Transmogrification.AllowMixedArmorTypes");
        sGameConfig->AddBoolConfig("Transmogrification.AllowMixedWeaponTypes");
        sGameConfig->AddBoolConfig("Transmogrification.AllowFishingPoles");
        sGameConfig->AddBoolConfig("Transmogrification.IgnoreReqRace");
        sGameConfig->AddBoolConfig("Transmogrification.IgnoreReqClass");
        sGameConfig->AddBoolConfig("Transmogrification.IgnoreReqSkill");
        sGameConfig->AddBoolConfig("Transmogrification.IgnoreReqSpell");
        sGameConfig->AddBoolConfig("Transmogrification.IgnoreReqLevel");
        sGameConfig->AddBoolConfig("Transmogrification.IgnoreReqEvent");
        sGameConfig->AddBoolConfig("Transmogrification.IgnoreReqStats");

        // Add int
        sGameConfig->AddIntConfig("Transmogrification.SetNpcText", 601084);
        sGameConfig->AddIntConfig("Transmogrification.MaxSets", 10);
        sGameConfig->AddIntConfig("Transmogrification.SetCopperCost");
        sGameConfig->AddIntConfig("Transmogrification.TransmogNpcText", 601083);
        sGameConfig->AddIntConfig("Transmogrification.CopperCost");
        sGameConfig->AddIntConfig("Transmogrification.TokenEntry", 49426);
        sGameConfig->AddIntConfig("Transmogrification.TokenAmount", 1);

        // Add float
        sGameConfig->AddFloatConfig("Transmogrification.SetCostModifier", 3.0f);
        sGameConfig->AddFloatConfig("Transmogrification.ScaledCostModifier");

        // Add string
        sGameConfig->AddStringConfig("Transmogrification.Allowed");
        sGameConfig->AddStringConfig("Transmogrification.NotAllowed");

        sTransmog->LoadConfig(reload);
    }

    void OnStartup() override
    {
        sTransmog->Init();
    }
};

class Transmogrification_Global : public GlobalScript
{
public:
    Transmogrification_Global() : GlobalScript("Transmogrification_Global") { }

    void OnItemDelFromDB(SQLTransaction& trans, uint32 itemGuid) override
    {
        sTransmog->DeleteFakeFromDB(itemGuid, &trans);
    }

    void OnMirrorImageDisplayItem(const Item* item, uint32& display) override
    {
        sTransmog->MirrorImageDisplayItem(item, display);        
    }
};

// Group all custom scripts
void AddSC_Transmogrification()
{
    new Transmogrification_Global();
    new Transmogrification_NPC();
    new Transmogrification_Player();
    new Transmogrification_World();
}
