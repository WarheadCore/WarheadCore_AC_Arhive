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
        if (CONF_GET_BOOL("Transmogrification.EnableTransmogInfo"))
            AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, sTransmog->GetGossipItemName(player, ITEM_HOW_WORKS), EQUIPMENT_SLOT_END + 9, 0);

        for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
        {
            if (!sTransmog->CanTransmogSlot(slot))
                continue;

            auto const& gossipIcon = sTransmog->GetGossipIcon(slot, player);
            if (gossipIcon.empty())
                continue;

            AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, gossipIcon, EQUIPMENT_SLOT_END, slot);
        }

        if (CONF_GET_BOOL("Transmogrification.EnableSets"))
            AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, sTransmog->GetGossipItemName(player, ITEM_MANAGE_SETS), EQUIPMENT_SLOT_END + 4, 0);

        AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, sTransmog->GetGossipItemName(player, ITEM_REMOVE_ALL_TRANSMOG), EQUIPMENT_SLOT_END + 2, 0, sTransmog->GetGossipItemName(player, ITEM_REMOVE_ALL_TRANSMOG_Q), 0, false);
        AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, sTransmog->GetGossipItemName(player, ITEM_UPDATE_MENU), EQUIPMENT_SLOT_END + 1, 0);

        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        ClearGossipMenuFor(player);

        switch (sender)
        {
            case EQUIPMENT_SLOT_END: // Show items you can use
                ShowTransmogItems(player, creature, action);
                break;
            case EQUIPMENT_SLOT_END + 1: // Main menu
                OnGossipHello(player, creature);
                break;
            case EQUIPMENT_SLOT_END + 2: // Remove Transmogrifications
                RemoveAllTransmogrifications(player);
                OnGossipHello(player, creature);
                break;
            case EQUIPMENT_SLOT_END + 3: // Remove Transmogrification from single item
                RemoveSingleTransmogrifications(player, action);
                OnGossipSelect(player, creature, EQUIPMENT_SLOT_END, action);
                break;
            case EQUIPMENT_SLOT_END + 4: // Presets menu
                ShowPresetsMenu(player, creature);
                break;
            case EQUIPMENT_SLOT_END + 5: // Use preset
                UsePreset(player, creature, action);
                break;
            case EQUIPMENT_SLOT_END + 6: // view preset
                ViewPreset(player, creature, action, sender);
                break;
            case EQUIPMENT_SLOT_END + 7: // Delete preset
                DeletePreset(player, creature, action);
                break;
            case EQUIPMENT_SLOT_END + 8: // Save preset
                SavePreset(player, creature, action, sender);
                break;
            case EQUIPMENT_SLOT_END + 10: // Set info
                AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, sTransmog->GetGossipItemName(player, ITEM_BACK), EQUIPMENT_SLOT_END + 4, 0);
                SendGossipMenuFor(player, CONF_GET_INT("Transmogrification.SetNpcText"), creature->GetGUID());
                break;
            case EQUIPMENT_SLOT_END + 9: // Transmog info
                AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, sTransmog->GetGossipItemName(player, ITEM_BACK), EQUIPMENT_SLOT_END + 1, 0);
                SendGossipMenuFor(player, CONF_GET_INT("Transmogrification.TransmogNpcText"), creature->GetGUID());
                break;
            default: // Transmogrify
                {
                    if (!sender && !action)
                    {
                        OnGossipHello(player, creature);
                        return true;
                    }

                    sTransmog->GossipTransmogrify(player, creature, action, sender);

                    CloseGossipMenuFor(player); // Wait for SetMoney to get fixed, issue #10053
                }
                break;
        }

        return true;
    }

    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, const char* code) override
    {
        ClearGossipMenuFor(player);

        if (sender || action)
            return true; // should never happen

        if (!CONF_GET_BOOL("Transmogrification.EnableSets"))
        {
            OnGossipHello(player, creature);
            return true;
        }

        sTransmog->SavePreset(player, creature, std::string(code));

        CloseGossipMenuFor(player); // Wait for SetMoney to get fixed, issue #10053

        return true;
    }

private:
    void ShowTransmogItems(Player* player, Creature* creature, uint8 slot) // Only checks bags while can use an item from anywhere in inventory
    {
        sTransmog->GossipShowTransmogItems(player, creature, slot);
    }

    void RemoveAllTransmogrifications(Player* player)
    {
        sTransmog->GossipRemoveAllTransmogrifications(player);
    }

    void RemoveSingleTransmogrifications(Player* player, uint32 const& action)
    {
        sTransmog->GossipRemoveSingleTransmogrifications(player, action);
    }

    void ShowPresetsMenu(Player* player, Creature* creature)
    {
        if (!CONF_GET_BOOL("Transmogrification.EnableSets"))
        {
            OnGossipHello(player, creature);
            return;
        }

        sTransmog->GossipShowPresetsMenu(player, creature);
    }

    void UsePreset(Player* player, Creature* creature, uint32 const& action)
    {
        if (!CONF_GET_BOOL("Transmogrification.EnableSets"))
        {
            OnGossipHello(player, creature);
            return;
        }

        sTransmog->GossipUsePreset(player, creature, action);

        OnGossipSelect(player, creature, EQUIPMENT_SLOT_END + 6, action);
    }

    void ViewPreset(Player* player, Creature* creature, uint32 const& action, uint32 const& sender)
    {
        if (!CONF_GET_BOOL("Transmogrification.EnableSets"))
        {
            OnGossipHello(player, creature);
            return;
        }

        sTransmog->GossipViewPreset(player, creature, action, sender);
    }

    void DeletePreset(Player* player, Creature* creature, uint32 const& action)
    {
        if (!CONF_GET_BOOL("Transmogrification.EnableSets"))
        {
            OnGossipHello(player, creature);
            return;
        }

        sTransmog->GossipDeletePreset(player, creature, action);

        OnGossipSelect(player, creature, EQUIPMENT_SLOT_END + 4, 0);
    }

    void SavePreset(Player* player, Creature* creature, uint32 const& action, uint32 const& sender)
    {
        if (!sTransmog->CanSavePresets(player))
        {
            OnGossipHello(player, creature);
            return;
        }

        sTransmog->GossipSavePreset(player, creature, action, sender);
    }
};

class Transmogrification_Player : public PlayerScript
{
public:
    Transmogrification_Player() : PlayerScript("Player_Transmogrify") { }

    void OnAfterSetVisibleItemSlot(Player* player, uint8 slot, Item* item) override
    {
        if (!item)
            return;

        if (uint32 entry = sTransmog->GetFakeEntry(item->GetGUID()))
            player->SetUInt32Value(PLAYER_VISIBLE_ITEM_1_ENTRYID + (slot * 2), entry);
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
        if (uint32 entry = sTransmog->GetFakeEntry(item->GetGUID()))
            display = uint32(sObjectMgr->GetItemTemplate(entry)->DisplayInfoID);
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
