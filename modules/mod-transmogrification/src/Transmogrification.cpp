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
#include "GameConfig.h"
#include "GameLocale.h"
#include "ModulesLocale.h"
#include "ScriptedGossip.h"
#include "GameEventMgr.h"

enum StringLocales : uint8
{
    // Slots
    TRANSMOG_LOCALE_HEAD = 1,
    TRANSMOG_LOCALE_SHOULDERS,
    TRANSMOG_LOCALE_SHIRT,
    TRANSMOG_LOCALE_CHEST,
    TRANSMOG_LOCALE_WAIST,
    TRANSMOG_LOCALE_LEGS,
    TRANSMOG_LOCALE_FEET,
    TRANSMOG_LOCALE_WRISTS,
    TRANSMOG_LOCALE_HANDS,
    TRANSMOG_LOCALE_BACK,
    TRANSMOG_LOCALE_MAIN_HAND,
    TRANSMOG_LOCALE_OFF_HAND,
    TRANSMOG_LOCALE_RANGED,
    TRANSMOG_LOCALE_TABARD,

    // Gossips
    TRANSMOG_LOCALE_GOSSIP_ITEM_HOW_WORKS,
    TRANSMOG_LOCALE_GOSSIP_ITEM_MANAGE_SETS,
    TRANSMOG_LOCALE_GOSSIP_ITEM_REMOVE_ALL_TRANSMOG,
    TRANSMOG_LOCALE_GOSSIP_ITEM_REMOVE_SINGLE_TRANSMOG,
    TRANSMOG_LOCALE_GOSSIP_ITEM_UPDATE_MENU,
    TRANSMOG_LOCALE_GOSSIP_ITEM_BACK,
    TRANSMOG_LOCALE_GOSSIP_ITEM_REMOVE_ALL_TRANSMOG_Q,
    TRANSMOG_LOCALE_GOSSIP_ITEM_REMOVE_SINGLE_TRANSMOG_Q,
    TRANSMOG_LOCALE_GOSSIP_ITEM_SAVE_SET,
    TRANSMOG_LOCALE_GOSSIP_ITEM_ITEM_BIND_Q,
    TRANSMOG_LOCALE_GOSSIP_ITEM_HOW_SET_WORKS,
    TRANSMOG_LOCALE_GOSSIP_ITEM_SET_USE,
    TRANSMOG_LOCALE_GOSSIP_ITEM_SET_DELETE,
    TRANSMOG_LOCALE_GOSSIP_ITEM_SET_DELETE_Q,
    TRANSMOG_LOCALE_GOSSIP_ITEM_SET_INSERT_NAME,

    // Strings
    TRANSMOG_LOCALE_TRANSMOG_OK,
    TRANSMOG_LOCALE_TRANSMOG_INVALID_SLOT,
    TRANSMOG_LOCALE_TRANSMOG_INVALID_SRC_ENTRY,
    TRANSMOG_LOCALE_TRANSMOG_MISSING_SRC_ITEM,
    TRANSMOG_LOCALE_TRANSMOG_MISSING_DEST_ITEM,
    TRANSMOG_LOCALE_TRANSMOG_INVALID_ITEMS,
    TRANSMOG_LOCALE_TRANSMOG_NOT_ENOUGH_MONEY,
    TRANSMOG_LOCALE_TRANSMOG_NOT_ENOUGH_TOKENS,
    TRANSMOG_LOCALE_UNTRANSMOG_OK,
    TRANSMOG_LOCALE_UNTRANSMOG_NO_TRANSMOGS,
    TRANSMOG_LOCALE_PRESET_ERR_INVALID_NAME,

    TRANSMOG_LOCALE_MAX
};

#define MODULE_NAME "mod-transmogrification"

Transmogrification* Transmogrification::instance()
{
    static Transmogrification instance;
    return &instance;
}

void Transmogrification::PresetTransmog(Player* player, Item* itemTransmogrified, uint32 fakeEntry, uint8 slot)
{
    if (!GetEnableSets())
        return;

    if (!player || !itemTransmogrified)
        return;

    if (slot >= EQUIPMENT_SLOT_END)
        return;

    if (!CanTransmogrifyItemWithItem(player, itemTransmogrified->GetTemplate(), sObjectMgr->GetItemTemplate(fakeEntry)))
        return;

    // [AZTH] Custom
    if (GetFakeEntry(itemTransmogrified->GetGUID()))
        DeleteFakeEntry(player, slot, itemTransmogrified);

    SetFakeEntry(player, fakeEntry, slot, itemTransmogrified); // newEntry

    itemTransmogrified->UpdatePlayedTime(player);
    itemTransmogrified->SetOwnerGUID(player->GetGUID());
    itemTransmogrified->SetNotRefundable(player);
    itemTransmogrified->ClearSoulboundTradeable(player);
}

void Transmogrification::LoadPlayerSets(uint64 pGUID)
{
    for (auto itr : _presetById[pGUID])
        itr.second.clear();

    _presetById[pGUID].clear();
    _presetByName[pGUID].clear();

    QueryResult result = CharacterDatabase.PQuery("SELECT `PresetID`, `SetName`, `SetData` FROM `custom_transmogrification_sets` WHERE Owner = %u", GUID_LOPART(pGUID));
    if (!result)
        return;

    do
    {
        uint8 PresetID = (*result)[0].GetUInt8();
        std::string SetName = (*result)[1].GetString();
        std::istringstream SetData((*result)[2].GetString());

        while (SetData.good())
        {
            uint32 slot;
            uint32 entry;

            SetData >> slot >> entry;
            if (SetData.fail())
                break;

            if (slot >= EQUIPMENT_SLOT_END)
            {
                LOG_ERROR("modules", "Item entry (FakeEntry: %u, playerGUID: %u, slot: %u, presetId: %u) has invalid slot, ignoring.", entry, GUID_LOPART(pGUID), slot, uint32(PresetID));
                continue;
            }

            if (sObjectMgr->GetItemTemplate(entry))
                _presetById[pGUID][PresetID][slot] = entry; // Transmogrification::Preset(presetName, fakeEntry);
        }

        if (!_presetById[pGUID][PresetID].empty())
            _presetByName[pGUID][PresetID] = SetName;
        else // should be deleted on startup, so  this never runs (shouldnt..)
        {
            _presetById[pGUID].erase(PresetID);
            CharacterDatabase.PExecute("DELETE FROM `custom_transmogrification_sets` WHERE Owner = %u AND PresetID = %u", GUID_LOPART(pGUID), PresetID);
        }
    } while (result->NextRow());
}

bool Transmogrification::GetEnableSets() const
{
    return EnableSets;
}

uint8 Transmogrification::GetMaxSets() const
{
    return MaxSets;
}

float Transmogrification::GetSetCostModifier() const
{
    return SetCostModifier;
}

int32 Transmogrification::GetSetCopperCost() const
{
    return SetCopperCost;
}

void Transmogrification::UnloadPlayerSets(uint64 pGUID)
{
    for (auto itr : _presetById[pGUID])
        itr.second.clear();

    _presetById[pGUID].clear();
    _presetByName[pGUID].clear();
}

std::string const Transmogrification::GetSlotName(Player* player, uint8 slot) const
{
    if (!CanTransmogSlot(slot))
        return "";

    uint8 localeIndex = static_cast<uint8>(player->GetSession()->GetSessionDbLocaleIndex());

    switch (slot)
    {
        case EQUIPMENT_SLOT_HEAD:
            return sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_HEAD, localeIndex).value_or("Head"); // Head
        case EQUIPMENT_SLOT_SHOULDERS:
            return sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_SHOULDERS, localeIndex).value_or("Shoulders"); // Shoulders
        case EQUIPMENT_SLOT_BODY:
            return sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_SHIRT, localeIndex).value_or("Shirt"); // Shirt
        case EQUIPMENT_SLOT_CHEST:
            return sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_CHEST, localeIndex).value_or("Chest"); // Chest
        case EQUIPMENT_SLOT_WAIST:
            return sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_WAIST, localeIndex).value_or("Waist"); // Waist
        case EQUIPMENT_SLOT_LEGS:
            return sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_LEGS, localeIndex).value_or("Legs"); // Legs
        case EQUIPMENT_SLOT_FEET:
            return sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_FEET, localeIndex).value_or("Feet"); // Feet
        case EQUIPMENT_SLOT_WRISTS:
            return sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_WRISTS, localeIndex).value_or("Wrists"); // Wrists
        case EQUIPMENT_SLOT_HANDS:
            return sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_HANDS, localeIndex).value_or("Hands"); // Hands
        case EQUIPMENT_SLOT_BACK:
            return sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_BACK, localeIndex).value_or("Back"); // Back
        case EQUIPMENT_SLOT_MAINHAND:
            return sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_MAIN_HAND, localeIndex).value_or("Main hand"); // Main hand
        case EQUIPMENT_SLOT_OFFHAND:
            return sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_OFF_HAND, localeIndex).value_or("Off hand"); // Off hand
        case EQUIPMENT_SLOT_RANGED:
            return sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_RANGED, localeIndex).value_or("Ranged"); // Ranged
        case EQUIPMENT_SLOT_TABARD:
            return sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_TABARD, localeIndex).value_or("Tabard"); // Tabard
        default:
            {
                LOG_FATAL("modules.transmog", "> Transmog: unknown slot (%u)", slot);
                return "";
            }
    }
}

std::string Transmogrification::GetItemIcon(uint32 entry, uint32 width, uint32 height, int x, int y) const
{
    const ItemTemplate* temp = sObjectMgr->GetItemTemplate(entry);
    const ItemDisplayInfoEntry* dispInfo = nullptr;

    std::ostringstream ss;
    ss << "|TInterface";

    if (temp)
    {
        dispInfo = sItemDisplayInfoStore.LookupEntry(temp->DisplayInfoID);
        if (dispInfo)
            ss << "/ICONS/" << dispInfo->inventoryIcon;
    }

    if (!dispInfo)
        ss << "/InventoryItems/WoWUnknownItem01";

    ss << ":" << width << ":" << height << ":" << x << ":" << y << "|t";

    return ss.str();
}

std::string Transmogrification::GetSlotIcon(uint8 slot, uint32 width, uint32 height, int x, int y) const
{
    std::ostringstream ss;
    ss << "|TInterface/PaperDoll/";

    switch (slot)
    {
        case EQUIPMENT_SLOT_HEAD:
            ss << "UI-PaperDoll-Slot-Head";
            break;
        case EQUIPMENT_SLOT_SHOULDERS:
            ss << "UI-PaperDoll-Slot-Shoulder";
            break;
        case EQUIPMENT_SLOT_BODY:
            ss << "UI-PaperDoll-Slot-Shirt";
            break;
        case EQUIPMENT_SLOT_CHEST:
            ss << "UI-PaperDoll-Slot-Chest";
            break;
        case EQUIPMENT_SLOT_WAIST:
            ss << "UI-PaperDoll-Slot-Waist";
            break;
        case EQUIPMENT_SLOT_LEGS:
            ss << "UI-PaperDoll-Slot-Legs";
            break;
        case EQUIPMENT_SLOT_FEET:
            ss << "UI-PaperDoll-Slot-Feet";
            break;
        case EQUIPMENT_SLOT_WRISTS:
            ss << "UI-PaperDoll-Slot-Wrists";
            break;
        case EQUIPMENT_SLOT_HANDS:
            ss << "UI-PaperDoll-Slot-Hands";
            break;
        case EQUIPMENT_SLOT_BACK:
            ss << "UI-PaperDoll-Slot-Chest";
            break;
        case EQUIPMENT_SLOT_MAINHAND:
            ss << "UI-PaperDoll-Slot-MainHand";
            break;
        case EQUIPMENT_SLOT_OFFHAND:
            ss << "UI-PaperDoll-Slot-SecondaryHand";
            break;
        case EQUIPMENT_SLOT_RANGED:
            ss << "UI-PaperDoll-Slot-Ranged";
            break;
        case EQUIPMENT_SLOT_TABARD:
            ss << "UI-PaperDoll-Slot-Tabard";
            break;
        default:
            ss << "UI-Backpack-EmptySlot";
    }

    ss << ":" << width << ":" << height << ":" << x << ":" << y << "|t";

    return ss.str();
}

std::string Transmogrification::GetItemLink(Item* item, WorldSession* session) const
{
    uint8 loc_idx = session->GetSessionDbLocaleIndex();
    const ItemTemplate* temp = item->GetTemplate();
    std::string name = temp->Name1;

    if (ItemLocale const* il = sGameLocale->GetItemLocale(temp->ItemId))
        sGameLocale->GetLocaleString(il->Name, loc_idx, name);

    if (int32 itemRandPropId = item->GetItemRandomPropertyId())
    {
        char* const* suffix = nullptr;

        if (itemRandPropId < 0)
        {
            const ItemRandomSuffixEntry* itemRandEntry = sItemRandomSuffixStore.LookupEntry(-item->GetItemRandomPropertyId());
            if (itemRandEntry)
                suffix = itemRandEntry->nameSuffix;
        }
        else
        {
            const ItemRandomPropertiesEntry* itemRandEntry = sItemRandomPropertiesStore.LookupEntry(item->GetItemRandomPropertyId());
            if (itemRandEntry)
                suffix = itemRandEntry->nameSuffix;
        }

        if (suffix)
        {
            std::string test(suffix[(name != temp->Name1) ? loc_idx : DEFAULT_LOCALE]);
            if (!test.empty())
            {
                name += ' ';
                name += test;
            }
        }
    }

    std::ostringstream oss;
    oss << "|c" << std::hex << ItemQualityColors[temp->Quality] << std::dec <<
        "|Hitem:" << temp->ItemId << ":" <<
        item->GetEnchantmentId(PERM_ENCHANTMENT_SLOT) << ":" <<
        item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT) << ":" <<
        item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_2) << ":" <<
        item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_3) << ":" <<
        item->GetEnchantmentId(BONUS_ENCHANTMENT_SLOT) << ":" <<
        item->GetItemRandomPropertyId() << ":" << item->GetItemSuffixFactor() << ":" <<
        (uint32)item->GetOwner()->getLevel() << "|h[" << name << "]|h|r";

    return oss.str();
}

std::string Transmogrification::GetItemLink(uint32 entry, WorldSession* session) const
{
    const ItemTemplate* temp = sObjectMgr->GetItemTemplate(entry);
    int loc_idx = session->GetSessionDbLocaleIndex();

    std::string name = temp->Name1;
    if (ItemLocale const* il = sGameLocale->GetItemLocale(entry))
        sGameLocale->GetLocaleString(il->Name, loc_idx, name);

    std::ostringstream oss;
    oss << "|c" << std::hex << ItemQualityColors[temp->Quality] << std::dec <<
        "|Hitem:" << entry << ":0:0:0:0:0:0:0:0:0|h[" << name << "]|h|r";

    return oss.str();
}

uint32 Transmogrification::GetFakeEntry(uint64 itemGUID) const
{
    auto const& itr = _dataMapStore.find(itemGUID);
    if (itr == _dataMapStore.end())
        return 0;

    auto const& itr2 = _mapStore.find(itr->second);
    if (itr2 == _mapStore.end())
        return 0;

    auto const& itr3 = itr2->second.find(itemGUID);
    if (itr3 == itr2->second.end())
        return 0;

    return itr3->second;
}

void Transmogrification::UpdateItem(Player* player, Item* item) const
{
    if (item->IsEquipped())
    {
        player->SetVisibleItemSlot(item->GetSlot(), item);

        if (player->IsInWorld())
            item->SendUpdateToPlayer(player);
    }
}

void Transmogrification::DeleteFakeEntry(Player* player, uint8 /*slot*/, Item* itemTransmogrified, SQLTransaction* trans)
{
    DeleteFakeFromDB(itemTransmogrified->GetGUID(), trans);
    UpdateItem(player, itemTransmogrified);
}

void Transmogrification::SetFakeEntry(Player* player, uint32 newEntry, uint8 /*slot*/, Item* itemTransmogrified)
{
    uint64 itemGUID = itemTransmogrified->GetGUID();

    _mapStore[player->GetGUID()][itemGUID] = newEntry;
    _dataMapStore[itemGUID] = player->GetGUID();

    CharacterDatabase.PExecute("REPLACE INTO custom_transmogrification (GUID, FakeEntry, Owner) VALUES (%u, %u, %u)", GUID_LOPART(itemGUID), newEntry, player->GetGUIDLow());
    UpdateItem(player, itemTransmogrified);
}

void Transmogrification::Transmogrify(Player* player, uint64 itemGUID, uint8 slot, bool no_cost)
{
    int32 cost = 0;

    // slot of the transmogrified item
    if (slot >= EQUIPMENT_SLOT_END)
    {
        LOG_DEBUG("modules.transmog", "WORLD: HandleTransmogrifyItems - Player (GUID: %u, name: %s) tried to transmogrify an item (lowguid: %u) with a wrong slot (%u) when transmogrifying items.",
                  player->GetGUIDLow(), player->GetName().c_str(), GUID_LOPART(itemGUID), slot);

        SendNotification(player, TRANSMOG_LOCALE_TRANSMOG_INVALID_SLOT);
        return;
    }

    Item* itemTransmogrifier = nullptr;

    // guid of the transmogrifier item, if it's not 0
    if (itemGUID)
    {
        itemTransmogrifier = player->GetItemByGuid(itemGUID);
        if (!itemTransmogrifier)
        {
            LOG_ERROR("modules.transmog", "WORLD: HandleTransmogrifyItems - Player (GUID: %u, name: %s) tried to transmogrify with an invalid item (lowguid: %u).", player->GetGUIDLow(), player->GetName().c_str(), GUID_LOPART(itemGUID));
            SendNotification(player, TRANSMOG_LOCALE_TRANSMOG_MISSING_SRC_ITEM);
            return;
        }
    }

    // transmogrified item
    Item* itemTransmogrified = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
    if (!itemTransmogrified)
    {
        LOG_DEBUG("modules", "WORLD: HandleTransmogrifyItems - Player (GUID: %u, name: %s) tried to transmogrify an invalid item in a valid slot (slot: %u).", player->GetGUIDLow(), player->GetName().c_str(), slot);
        SendNotification(player, TRANSMOG_LOCALE_TRANSMOG_MISSING_DEST_ITEM);
        return;
    }

    if (!itemTransmogrifier) // reset look newEntry
    {
        // Custom
        DeleteFakeEntry(player, slot, itemTransmogrified);
    }
    else
    {
        if (!CanTransmogrifyItemWithItem(player, itemTransmogrified->GetTemplate(), itemTransmogrifier->GetTemplate()))
        {
            LOG_DEBUG("modules", "WORLD: HandleTransmogrifyItems - Player (GUID: %u, name: %s) failed CanTransmogrifyItemWithItem (%u with %u).",
                      player->GetGUIDLow(), player->GetName().c_str(), itemTransmogrified->GetEntry(), itemTransmogrifier->GetEntry());

            SendNotification(player, TRANSMOG_LOCALE_TRANSMOG_INVALID_ITEMS);
            return;
        }

        if (!no_cost)
        {
            if (GetRequireToken())
            {
                if (player->HasItemCount(TokenEntry, TokenAmount))
                    player->DestroyItemCount(TokenEntry, TokenAmount, true);
                else
                {
                    SendNotification(player, TRANSMOG_LOCALE_TRANSMOG_NOT_ENOUGH_TOKENS);
                    return;
                }
            }

            cost = GetSpecialPrice(itemTransmogrified->GetTemplate());
            cost *= GetScaledCostModifier();
            cost += GetCopperCost();

            if (cost) // 0 cost if reverting look
            {
                if (cost < 0)
                    LOG_DEBUG("modules", "Transmogrification::Transmogrify - %s (%u) transmogrification invalid cost (non negative, amount %i). Transmogrified %u with %u",
                              player->GetName().c_str(), player->GetGUIDLow(), -cost, itemTransmogrified->GetEntry(), itemTransmogrifier->GetEntry());
                else
                {
                    if (!player->HasEnoughMoney(cost))
                    {
                        SendNotification(player, TRANSMOG_LOCALE_TRANSMOG_NOT_ENOUGH_MONEY);
                        return;
                    }

                    player->ModifyMoney(-cost, false);
                }
            }
        }

        // Custom
        SetFakeEntry(player, itemTransmogrifier->GetEntry(), slot, itemTransmogrified); // newEntry

        itemTransmogrified->UpdatePlayedTime(player);
        itemTransmogrified->SetOwnerGUID(player->GetGUID());
        itemTransmogrified->SetNotRefundable(player);
        itemTransmogrified->ClearSoulboundTradeable(player);

        if (itemTransmogrifier->GetTemplate()->Bonding == BIND_WHEN_EQUIPED || itemTransmogrifier->GetTemplate()->Bonding == BIND_WHEN_USE)
            itemTransmogrifier->SetBinding(true);

        itemTransmogrifier->SetOwnerGUID(player->GetGUID());
        itemTransmogrifier->SetNotRefundable(player);
        itemTransmogrifier->ClearSoulboundTradeable(player);
    }

    SendNotification(player, TRANSMOG_LOCALE_TRANSMOG_OK);
}

bool Transmogrification::CanTransmogrifyItemWithItem(Player* player, ItemTemplate const* target, ItemTemplate const* source) const
{
    if (!target || !source)
        return false;

    if (source->ItemId == target->ItemId)
        return false;

    if (source->DisplayInfoID == target->DisplayInfoID)
        return false;

    if (source->Class != target->Class)
        return false;

    if (source->InventoryType == INVTYPE_BAG ||
            source->InventoryType == INVTYPE_RELIC ||
            source->InventoryType == INVTYPE_FINGER ||
            source->InventoryType == INVTYPE_TRINKET ||
            source->InventoryType == INVTYPE_AMMO ||
            source->InventoryType == INVTYPE_QUIVER)
        return false;

    if (target->InventoryType == INVTYPE_BAG ||
            target->InventoryType == INVTYPE_RELIC ||
            target->InventoryType == INVTYPE_FINGER ||
            target->InventoryType == INVTYPE_TRINKET ||
            target->InventoryType == INVTYPE_AMMO ||
            target->InventoryType == INVTYPE_QUIVER)
        return false;

    if (!SuitableForTransmogrification(player, target) || !SuitableForTransmogrification(player, source)) // if (!transmogrified->CanTransmogrify() || !transmogrifier->CanBeTransmogrified())
        return false;

    if (IsRangedWeapon(source->Class, source->SubClass) != IsRangedWeapon(target->Class, target->SubClass))
        return false;

    if (source->SubClass != target->SubClass && !IsRangedWeapon(target->Class, target->SubClass) && !IsAllowed(source->ItemId))
    {
        if (source->Class == ITEM_CLASS_ARMOR && !AllowMixedArmorTypes)
            return false;

        /*if (source->Class == ITEM_CLASS_WEAPON) && !AllowMixedWeaponTypes)
            return false;*/
        if (source->Class == ITEM_CLASS_WEAPON && !player->GetSkillValue(target->GetSkill()))
            return false;
    }

    if (!AllowMixedArmorTypes && source->InventoryType != target->InventoryType)
    {
        if (source->Class == ITEM_CLASS_ARMOR &&
                !((source->InventoryType == INVTYPE_CHEST || source->InventoryType == INVTYPE_ROBE) &&
                  (target->InventoryType == INVTYPE_CHEST || target->InventoryType == INVTYPE_ROBE)))
            return false;
    }

    if (!AllowMixedWeaponTypes && source->InventoryType != target->InventoryType)
    {
        if (target->Class != ITEM_CLASS_WEAPON || source->Class != ITEM_CLASS_WEAPON)
            return false;

        if (source->InventoryType == INVTYPE_2HWEAPON || target->InventoryType == INVTYPE_2HWEAPON)
            return false;
    }

    if (AllowMixedWeaponTypes && source->InventoryType != target->InventoryType)
    {
        if (source->Class == ITEM_CLASS_WEAPON && !(IsRangedWeapon(target->Class, target->SubClass) ||
                (
                    // [AZTH] Yehonal: fixed weapon check
                    (target->InventoryType == INVTYPE_WEAPON ||
                     target->InventoryType == INVTYPE_2HWEAPON ||
                     target->InventoryType == INVTYPE_WEAPONMAINHAND ||
                     target->InventoryType == INVTYPE_WEAPONOFFHAND)
                    &&
                    (source->InventoryType == INVTYPE_WEAPON ||
                     source->InventoryType == INVTYPE_2HWEAPON ||
                     source->InventoryType == INVTYPE_WEAPONMAINHAND ||
                     source->InventoryType == INVTYPE_WEAPONOFFHAND))
                                                   ))
            return false;
    }

    return true;
}

bool Transmogrification::SuitableForTransmogrification(Player* player, ItemTemplate const* proto) const
{
    // ItemTemplate const* proto = item->GetTemplate();
    if (!player || !proto)
        return false;

    if (proto->Class != ITEM_CLASS_ARMOR &&
            proto->Class != ITEM_CLASS_WEAPON)
        return false;

    // Skip all checks for allowed items
    if (IsAllowed(proto->ItemId))
        return true;

    //[AZTH] Yehonal
    if (proto->SubClass && !player->GetSkillValue(proto->GetSkill()))
    {
        if (proto->Class == ITEM_CLASS_ARMOR && !AllowMixedArmorTypes)
            return false;
        else if (proto->Class == ITEM_CLASS_WEAPON && !AllowMixedWeaponTypes)
            return false;
        else
            return false;
    }

    if (IsNotAllowed(proto->ItemId))
        return false;

    if (!AllowFishingPoles && proto->Class == ITEM_CLASS_WEAPON && proto->SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE)
        return false;

    if (!IsAllowedQuality(proto->Quality)) // (proto->Quality == ITEM_QUALITY_LEGENDARY)
        return false;

    if ((proto->Flags2 & ITEM_FLAGS_EXTRA_HORDE_ONLY) && player->GetTeamId() != TEAM_HORDE)
        return false;

    if ((proto->Flags2 & ITEM_FLAGS_EXTRA_ALLIANCE_ONLY) && player->GetTeamId() != TEAM_ALLIANCE)
        return false;

    if (!IgnoreReqClass && (proto->AllowableClass & player->getClassMask()) == 0)
        return false;

    if (!IgnoreReqRace && (proto->AllowableRace & player->getRaceMask()) == 0)
        return false;

    if (!IgnoreReqSkill && proto->RequiredSkill != 0 &&
            (!player->GetSkillValue(proto->RequiredSkill) || player->GetSkillValue(proto->RequiredSkill) < proto->RequiredSkillRank))
        return false;

    if (!IgnoreReqSpell && proto->RequiredSpell != 0 && !player->HasSpell(proto->RequiredSpell))
        return false;

    if (!IgnoreReqLevel && player->getLevel() < proto->RequiredLevel)
        return false;

    // If World Event is not active, prevent using event dependant items
    if (!IgnoreReqEvent && proto->HolidayId && !IsHolidayActive((HolidayIds)proto->HolidayId))
        return false;

    if (!IgnoreReqStats)
    {
        if (!proto->RandomProperty && !proto->RandomSuffix
                /*[AZTH] Yehonal: we should transmorg also items without stats*/
                && proto->StatsCount > 0)
        {
            bool found = false;

            for (uint8 i = 0; i < proto->StatsCount; ++i)
            {
                if (proto->ItemStat[i].ItemStatValue != 0)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
                return false;
        }
    }

    return true;
}

uint32 Transmogrification::GetSpecialPrice(ItemTemplate const* proto) const
{
    uint32 cost = proto->SellPrice < 10000 ? 10000 : proto->SellPrice;
    return cost;
}

bool Transmogrification::IsRangedWeapon(uint32 Class, uint32 SubClass) const
{
    return Class == ITEM_CLASS_WEAPON && (
               SubClass == ITEM_SUBCLASS_WEAPON_BOW ||
               SubClass == ITEM_SUBCLASS_WEAPON_GUN ||
               SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW);
}

bool Transmogrification::IsAllowed(uint32 entry) const
{
    return Allowed.find(entry) != Allowed.end();
}

bool Transmogrification::IsNotAllowed(uint32 entry) const
{
    return NotAllowed.find(entry) != NotAllowed.end();
}

bool Transmogrification::IsAllowedQuality(uint32 quality) const
{
    switch (quality)
    {
        case ITEM_QUALITY_POOR:
            return AllowPoor;
        case ITEM_QUALITY_NORMAL:
            return AllowCommon;
        case ITEM_QUALITY_UNCOMMON:
            return AllowUncommon;
        case ITEM_QUALITY_RARE:
            return AllowRare;
        case ITEM_QUALITY_EPIC:
            return AllowEpic;
        case ITEM_QUALITY_LEGENDARY:
            return AllowLegendary;
        case ITEM_QUALITY_ARTIFACT:
            return AllowArtifact;
        case ITEM_QUALITY_HEIRLOOM:
            return AllowHeirloom;
        default:
            return false;
    }
}

void Transmogrification::LoadConfig(bool reload)
{
    EnableSetInfo = sGameConfig->GetBoolConfig("Transmogrification.EnableSetInfo");
    SetNpcText = uint32(sGameConfig->GetIntConfig("Transmogrification.SetNpcText"));
    EnableSets = sGameConfig->GetBoolConfig("Transmogrification.EnableSets");

    SetCostModifier = sGameConfig->GetFloatConfig("Transmogrification.SetCostModifier");
    SetCopperCost = sGameConfig->GetIntConfig("Transmogrification.SetCopperCost");

    MaxSets = (uint8)sGameConfig->GetIntConfig("Transmogrification.MaxSets");
    if (MaxSets > MAX_OPTIONS)
        MaxSets = MAX_OPTIONS;

    EnableTransmogInfo = sGameConfig->GetBoolConfig("Transmogrification.EnableTransmogInfo");
    TransmogNpcText = uint32(sGameConfig->GetIntConfig("Transmogrification.TransmogNpcText"));

    std::istringstream issAllowed(sGameConfig->GetStringConfig("Transmogrification.Allowed"));
    std::istringstream issNotAllowed(sGameConfig->GetStringConfig("Transmogrification.NotAllowed"));

    while (issAllowed.good())
    {
        uint32 entry;
        issAllowed >> entry;

        if (issAllowed.fail())
            break;

        Allowed.insert(entry);
    }

    while (issNotAllowed.good())
    {
        uint32 entry;
        issNotAllowed >> entry;

        if (issNotAllowed.fail())
            break;

        NotAllowed.insert(entry);
    }

    ScaledCostModifier = sGameConfig->GetFloatConfig("Transmogrification.ScaledCostModifier");
    CopperCost = sGameConfig->GetIntConfig("Transmogrification.CopperCost");

    RequireToken = sGameConfig->GetBoolConfig("Transmogrification.RequireToken");
    TokenAmount = uint32(sGameConfig->GetIntConfig("Transmogrification.TokenAmount"));

    AllowPoor = sGameConfig->GetBoolConfig("Transmogrification.AllowPoor");
    AllowCommon = sGameConfig->GetBoolConfig("Transmogrification.AllowCommon");
    AllowUncommon = sGameConfig->GetBoolConfig("Transmogrification.AllowUncommon");
    AllowRare = sGameConfig->GetBoolConfig("Transmogrification.AllowRare");
    AllowEpic = sGameConfig->GetBoolConfig("Transmogrification.AllowEpic");
    AllowLegendary = sGameConfig->GetBoolConfig("Transmogrification.AllowLegendary");
    AllowArtifact = sGameConfig->GetBoolConfig("Transmogrification.AllowArtifact");
    AllowHeirloom = sGameConfig->GetBoolConfig("Transmogrification.AllowHeirloom");

    AllowMixedArmorTypes = sGameConfig->GetBoolConfig("Transmogrification.AllowMixedArmorTypes");
    AllowMixedWeaponTypes = sGameConfig->GetBoolConfig("Transmogrification.AllowMixedWeaponTypes");
    AllowFishingPoles = sGameConfig->GetBoolConfig("Transmogrification.AllowFishingPoles");

    IgnoreReqRace = sGameConfig->GetBoolConfig("Transmogrification.IgnoreReqRace");
    IgnoreReqClass = sGameConfig->GetBoolConfig("Transmogrification.IgnoreReqClass");
    IgnoreReqSkill = sGameConfig->GetBoolConfig("Transmogrification.IgnoreReqSkill");
    IgnoreReqSpell = sGameConfig->GetBoolConfig("Transmogrification.IgnoreReqSpell");
    IgnoreReqLevel = sGameConfig->GetBoolConfig("Transmogrification.IgnoreReqLevel");
    IgnoreReqEvent = sGameConfig->GetBoolConfig("Transmogrification.IgnoreReqEvent");
    IgnoreReqStats = sGameConfig->GetBoolConfig("Transmogrification.IgnoreReqStats");

    if (reload) // dont store presets for nothing
    {
        for (auto const& itr : sWorld->GetAllSessions())
        {
            if (Player* player = itr.second->GetPlayer())
            {
                // skipping session check
                UnloadPlayerSets(player->GetGUID());

                if (GetEnableSets())
                    LoadPlayerSets(player->GetGUID());
            }
        }
    }

    if (!reload)
    {
        LOG_DEBUG("modules.transmog", "Deleting non-existing transmogrification entries...");
        CharacterDatabase.Execute("DELETE FROM custom_transmogrification WHERE NOT EXISTS (SELECT 1 FROM item_instance WHERE item_instance.guid = custom_transmogrification.GUID)");

        // Clean even if disabled
        // Dont delete even if player has more presets than should
        CharacterDatabase.Execute("DELETE FROM `custom_transmogrification_sets` WHERE NOT EXISTS(SELECT 1 FROM characters WHERE characters.guid = custom_transmogrification_sets.Owner)");
    }
}

void Transmogrification::Init(bool reload)
{
    if (sModulesLocale->GetStringsCount(MODULE_NAME) != StringLocales::TRANSMOG_LOCALE_MAX - 1)
    {
        LOG_FATAL("modules.transmog", "> AntiAD: string locales (%u) for module != (%u)", sModulesLocale->GetStringsCount(MODULE_NAME), StringLocales::TRANSMOG_LOCALE_MAX - 1);
        return;
    }

    TokenEntry = uint32(sGameConfig->GetIntConfig("Transmogrification.TokenEntry"));

    if (!sObjectMgr->GetItemTemplate(TokenEntry))
    {
        LOG_ERROR("modules.transmog", "Transmogrification.TokenEntry (%u) does not exist. Using default.", TokenEntry);
        TokenEntry = 49426;
    }
}

void Transmogrification::DeleteFakeFromDB(uint64 itemGUID, SQLTransaction* trans)
{
    if (_dataMapStore.find(itemGUID) != _dataMapStore.end())
    {
        if (_mapStore.find(_dataMapStore[itemGUID]) != _mapStore.end())
            _mapStore[_dataMapStore[itemGUID]].erase(itemGUID);

        _dataMapStore.erase(itemGUID);
    }

    if (trans)
        (*trans)->PAppend("DELETE FROM custom_transmogrification WHERE GUID = %u", GUID_LOPART(itemGUID));
    else
        CharacterDatabase.PExecute("DELETE FROM custom_transmogrification WHERE GUID = %u", GUID_LOPART(itemGUID));
}

bool Transmogrification::GetEnableTransmogInfo() const
{
    return EnableTransmogInfo;
}

uint32 Transmogrification::GetTransmogNpcText() const
{
    return TransmogNpcText;
}

bool Transmogrification::GetEnableSetInfo() const
{
    return EnableSetInfo;
}

uint32 Transmogrification::GetSetNpcText() const
{
    return SetNpcText;
}

float Transmogrification::GetScaledCostModifier() const
{
    return ScaledCostModifier;
}

int32 Transmogrification::GetCopperCost() const
{
    return CopperCost;
}

bool Transmogrification::GetRequireToken() const
{
    return RequireToken;
}

uint32 Transmogrification::GetTokenEntry() const
{
    return TokenEntry;
}

uint32 Transmogrification::GetTokenAmount() const
{
    return TokenAmount;
}

bool Transmogrification::GetAllowMixedArmorTypes() const
{
    return AllowMixedArmorTypes;
}

bool Transmogrification::GetAllowMixedWeaponTypes() const
{
    return AllowMixedWeaponTypes;
}

bool Transmogrification::CanTransmogSlot(uint8 slot) const
{
    if (slot == EQUIPMENT_SLOT_HEAD ||
            slot == EQUIPMENT_SLOT_SHOULDERS ||
            slot == EQUIPMENT_SLOT_BODY ||
            slot == EQUIPMENT_SLOT_CHEST ||
            slot == EQUIPMENT_SLOT_WAIST ||
            slot == EQUIPMENT_SLOT_LEGS ||
            slot == EQUIPMENT_SLOT_FEET ||
            slot == EQUIPMENT_SLOT_WRISTS ||
            slot == EQUIPMENT_SLOT_HANDS ||
            slot == EQUIPMENT_SLOT_BACK ||
            slot == EQUIPMENT_SLOT_MAINHAND ||
            slot == EQUIPMENT_SLOT_OFFHAND ||
            slot == EQUIPMENT_SLOT_RANGED ||
            slot == EQUIPMENT_SLOT_TABARD)
        return true;

    return false;
}

void Transmogrification::ClearPlayerAtLogout(Player* player)
{
    if (!player)
        return;

    uint64 pGUID = player->GetGUID();

    for (auto const& itr : _mapStore[pGUID])
        _dataMapStore.erase(itr.first);

    _mapStore.erase(pGUID);

    if (GetEnableSets())
        UnloadPlayerSets(pGUID);
}

void Transmogrification::LoadPlayerAtLogin(Player* player)
{
    if (!player)
        return;

    uint64 playerGUID = player->GetGUID();

    _mapStore.erase(playerGUID);

    QueryResult result = CharacterDatabase.PQuery("SELECT GUID, FakeEntry FROM custom_transmogrification WHERE Owner = %u", player->GetGUIDLow());
    if (result)
    {
        do
        {
            uint64 itemGUID = MAKE_NEW_GUID((*result)[0].GetUInt32(), 0, HIGHGUID_ITEM);
            uint32 fakeEntry = (*result)[1].GetUInt32();

            if (sObjectMgr->GetItemTemplate(fakeEntry))
            {
                _dataMapStore[itemGUID] = playerGUID;
                _mapStore[playerGUID][itemGUID] = fakeEntry;
            }

        } while (result->NextRow());

        for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
        {
            if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                player->SetVisibleItemSlot(slot, item);
        }
    }

    if (GetEnableSets())
        LoadPlayerSets(playerGUID);
}

std::string const Transmogrification::GetGossipIcon(uint8 slot, Player* player)
{
    auto const& slotName = GetSlotName(player, slot);
    if (slotName.empty())
        return "";

    Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
    uint32 entry = newItem ? GetFakeEntry(newItem->GetGUID()) : 0;
    std::string icon = entry ? GetItemIcon(entry, 30, 30, -18, 0) : GetSlotIcon(slot, 30, 30, -18, 0);

    return icon + slotName;
}

std::string const Transmogrification::GetGossipItemName(Player* player, TransmogrificationGossipItem gossipItem)
{
    uint8 localeIndex = static_cast<uint8>(player->GetSession()->GetSessionDbLocaleIndex());

    switch (gossipItem)
    {
        case ITEM_HOW_WORKS:
            return *sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_GOSSIP_ITEM_HOW_WORKS, localeIndex);
        case ITEM_MANAGE_SETS:
            return *sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_GOSSIP_ITEM_MANAGE_SETS, localeIndex);
        case ITEM_REMOVE_ALL_TRANSMOG:
            return *sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_GOSSIP_ITEM_REMOVE_ALL_TRANSMOG, localeIndex);
        case ITEM_REMOVE_SINGLE_TRANSMOG:
            return *sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_GOSSIP_ITEM_REMOVE_SINGLE_TRANSMOG, localeIndex);
        case ITEM_UPDATE_MENU:
            return *sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_GOSSIP_ITEM_UPDATE_MENU, localeIndex);
        case ITEM_BACK:
            return *sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_GOSSIP_ITEM_BACK, localeIndex);
        case ITEM_REMOVE_ALL_TRANSMOG_Q:
            return *sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_GOSSIP_ITEM_REMOVE_ALL_TRANSMOG_Q, localeIndex);
        case ITEM_REMOVE_SINGLE_TRANSMOG_Q:
            return *sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_GOSSIP_ITEM_REMOVE_SINGLE_TRANSMOG_Q, localeIndex);
        case ITEM_SAVE_SET:
            return *sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_GOSSIP_ITEM_SAVE_SET, localeIndex);
        case ITEM_ITEM_BIND_Q:
            return *sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_GOSSIP_ITEM_ITEM_BIND_Q, localeIndex);
        case ITEM_HOW_SET_WORKS:
            return *sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_GOSSIP_ITEM_HOW_SET_WORKS, localeIndex);
        case ITEM_SET_USE:
            return *sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_GOSSIP_ITEM_SET_USE, localeIndex);
        case ITEM_SET_DELETE:
            return *sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_GOSSIP_ITEM_SET_DELETE, localeIndex);
        case ITEM_SET_DELETE_Q:
            return *sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_GOSSIP_ITEM_SET_DELETE_Q, localeIndex);
        case ITEM_SET_INSERT_NAME:
            return *sModulesLocale->GetModuleString(MODULE_NAME, TRANSMOG_LOCALE_GOSSIP_ITEM_SET_INSERT_NAME, localeIndex);
        default:
            return "";
    }
}

bool Transmogrification::CanSavePresets(Player* player)
{
    return GetEnableSets() && static_cast<uint8>(_presetByName[player->GetGUID()].size()) < GetMaxSets();
}

void Transmogrification::SavePreset(Player* player, Creature* creature, std::string const& name)
{
    if (name.find('"') != std::string::npos || name.find('\\') != std::string::npos)
    {
        SendNotification(player, TRANSMOG_LOCALE_PRESET_ERR_INVALID_NAME);
        return;
    }

    for (uint8 presetID = 0; presetID < GetMaxSets(); ++presetID) // should never reach over max
    {
        if (_presetByName[player->GetGUID()].find(presetID) != _presetByName[player->GetGUID()].end())
            continue; // Just remember never to use _presetByName[pGUID][presetID] when finding etc!

        int32 cost = 0;
        std::map<uint8, uint32> items;

        for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
        {
            if (GetSlotName(player, slot).empty())
                continue;

            if (Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
            {
                uint32 entry = GetFakeEntry(newItem->GetGUID());
                if (!entry)
                    continue;

                const ItemTemplate* temp = sObjectMgr->GetItemTemplate(entry);

                if (!temp)
                    continue;

                if (!SuitableForTransmogrification(player, temp))
                    continue;

                cost += GetSpecialPrice(temp);
                items[slot] = entry;
            }
        }

        if (items.empty())
            break; // no transmogrified items were found to be saved

        cost *= GetSetCostModifier();
        cost += GetSetCopperCost();

        if (!player->HasEnoughMoney(cost))
        {
            SendNotification(player, TRANSMOG_LOCALE_TRANSMOG_NOT_ENOUGH_MONEY);
            break;
        }

        std::ostringstream ss;

        for (auto const& itr : items)
        {
            ss << uint32(itr.first) << ' ' << itr.second << ' ';
            _presetById[player->GetGUID()][presetID][itr.first] = itr.second;
        }

        _presetByName[player->GetGUID()][presetID] = name; // Make sure code doesnt mess up SQL!
        CharacterDatabase.PExecute("REPLACE INTO `custom_transmogrification_sets` (`Owner`, `PresetID`, `SetName`, `SetData`) VALUES (%u, %u, \"%s\", \"%s\")", player->GetGUIDLow(), uint32(presetID), name.c_str(), ss.str().c_str());

        if (cost)
            player->ModifyMoney(-cost);

        break;
    }
}

void Transmogrification::GossipShowTransmogItems(Player* player, Creature* creature, uint8 slot)
{
    if (!player || !creature)
        return;

    auto session = player->GetSession();

    Item* oldItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
    if (oldItem)
    {
        uint32 limit = 0;
        uint32 price = GetSpecialPrice(oldItem->GetTemplate());

        price *= GetScaledCostModifier();
        price += GetCopperCost();

        std::ostringstream ss;
        ss << std::endl;
        if (GetRequireToken())
            ss << std::endl << std::endl << GetTokenAmount() << " x " << GetItemLink(GetTokenEntry(), session);

        for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
        {
            if (limit > MAX_OPTIONS)
                break;

            Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (!newItem)
                continue;

            if (!CanTransmogrifyItemWithItem(player, oldItem->GetTemplate(), newItem->GetTemplate()))
                continue;

            if (GetFakeEntry(oldItem->GetGUID()) == newItem->GetEntry())
                continue;

            ++limit;

            AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetItemIcon(newItem->GetEntry(), 30, 30, -18, 0) + GetItemLink(newItem, session), slot, newItem->GetGUIDLow(),
                             GetGossipItemName(player, ITEM_ITEM_BIND_Q) + GetItemIcon(newItem->GetEntry(), 40, 40, -15, -10) + GetItemLink(newItem, session) + ss.str(), price, false);
        }

        for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
        {
            Bag* bag = player->GetBagByPos(i);
            if (!bag)
                continue;

            for (uint32 j = 0; j < bag->GetBagSize(); ++j)
            {
                if (limit > MAX_OPTIONS)
                    break;

                Item* newItem = player->GetItemByPos(i, j);
                if (!newItem)
                    continue;

                if (!CanTransmogrifyItemWithItem(player, oldItem->GetTemplate(), newItem->GetTemplate()))
                    continue;

                if (GetFakeEntry(oldItem->GetGUID()) == newItem->GetEntry())
                    continue;

                ++limit;

                AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetItemIcon(newItem->GetEntry(), 30, 30, -18, 0) + GetItemLink(newItem, session), slot, newItem->GetGUIDLow(),
                                 GetGossipItemName(player, ITEM_ITEM_BIND_Q) + GetItemIcon(newItem->GetEntry(), 40, 40, -15, -10) + GetItemLink(newItem, session) + ss.str(), price, false);
            }
        }
    }

    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetGossipItemName(player, ITEM_REMOVE_SINGLE_TRANSMOG), EQUIPMENT_SLOT_END + 3, slot, GetGossipItemName(player, ITEM_REMOVE_SINGLE_TRANSMOG_Q), 0, false);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetGossipItemName(player, ITEM_UPDATE_MENU), EQUIPMENT_SLOT_END, slot);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetGossipItemName(player, ITEM_BACK), EQUIPMENT_SLOT_END + 1, 0);

    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
}

void Transmogrification::GossipRemoveAllTransmogrifications(Player* player)
{
    bool removed = false;
    auto session = player->GetSession();

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
    {
        if (Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
        {
            if (!GetFakeEntry(newItem->GetGUID()))
                continue;

            DeleteFakeEntry(player, slot, newItem, &trans);
            removed = true;
        }
    }

    if (removed)
    {
        SendNotification(player, TRANSMOG_LOCALE_UNTRANSMOG_OK);
        CharacterDatabase.CommitTransaction(trans);
    }
    else
        SendNotification(player, TRANSMOG_LOCALE_UNTRANSMOG_NO_TRANSMOGS);
}

void Transmogrification::GossipRemoveSingleTransmogrifications(Player* player, uint32 const& action)
{
    auto session = player->GetSession();

    Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, action);
    if (!newItem)
        return;

    if (GetFakeEntry(newItem->GetGUID()))
    {
        DeleteFakeEntry(player, action, newItem);
        SendNotification(player, TRANSMOG_LOCALE_UNTRANSMOG_OK);
    }
    else
        SendNotification(player, TRANSMOG_LOCALE_UNTRANSMOG_NO_TRANSMOGS);
}

void Transmogrification::GossipShowPresetsMenu(Player* player, Creature* creature)
{
    if (GetEnableSetInfo())
        AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetGossipItemName(player, ITEM_HOW_SET_WORKS), EQUIPMENT_SLOT_END + 10, 0);

    for (auto const& itr : _presetByName[player->GetGUID()])
        AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Statue_02:30:30:-18:0|t" + itr.second, EQUIPMENT_SLOT_END + 6, itr.first);

    if (_presetByName[player->GetGUID()].size() < GetMaxSets())
        AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetGossipItemName(player, ITEM_SAVE_SET), EQUIPMENT_SLOT_END + 8, 0);

    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetGossipItemName(player, ITEM_BACK), EQUIPMENT_SLOT_END + 1, 0);
    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
}

void Transmogrification::GossipUsePreset(Player* player, Creature* creature, uint32 const& action)
{
    // action = presetID
    for (auto const& itr : _presetById[player->GetGUID()][action])
    {
        if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, itr.first))
            PresetTransmog(player, item, itr.second, itr.first);
    }
}

void Transmogrification::GossipViewPreset(Player* player, Creature* creature, uint32 const& action, uint32 const& sender)
{
    auto session = player->GetSession();

    // action = presetID
    for (auto const& itr : _presetById[player->GetGUID()][action])
        AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetItemIcon(itr.second, 30, 30, -18, 0) + GetItemLink(itr.second, session), sender, action);

    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetGossipItemName(player, ITEM_SET_USE), EQUIPMENT_SLOT_END + 5, action,
                     GetGossipItemName(player, ITEM_ITEM_BIND_Q) + _presetByName[player->GetGUID()][action], 0, false);

    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetGossipItemName(player, ITEM_SET_DELETE), EQUIPMENT_SLOT_END + 7, action, GetGossipItemName(player, ITEM_SET_DELETE_Q) + _presetByName[player->GetGUID()][action] + "?", 0, false);

    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetGossipItemName(player, ITEM_BACK), EQUIPMENT_SLOT_END + 4, 0);
    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
}

void Transmogrification::GossipDeletePreset(Player* player, Creature* creature, uint32 const& action)
{
    // action = presetID
    CharacterDatabase.PExecute("DELETE FROM `custom_transmogrification_sets` WHERE Owner = %u AND PresetID = %u", player->GetGUIDLow(), action);
    _presetById[player->GetGUID()][action].clear();
    _presetById[player->GetGUID()].erase(action);
    _presetByName[player->GetGUID()].erase(action);
}

void Transmogrification::GossipSavePreset(Player* player, Creature* creature, uint32 const& action, uint32 const& sender)
{
    uint32 cost = 0;
    bool canSave = false;
    auto session = player->GetSession();

    for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
    {
        if (GetSlotName(player, slot).empty())
            continue;

        if (Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
        {
            uint32 entry = GetFakeEntry(newItem->GetGUID());
            if (!entry)
                continue;

            const ItemTemplate* temp = sObjectMgr->GetItemTemplate(entry);
            if (!temp)
                continue;

            if (!SuitableForTransmogrification(player, temp)) // no need to check?
                continue;

            cost += GetSpecialPrice(temp);
            canSave = true;

            AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetItemIcon(entry, 30, 30, -18, 0) + GetItemLink(entry, session), EQUIPMENT_SLOT_END + 8, 0);
        }
    }

    if (canSave)
        AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetGossipItemName(player, ITEM_SAVE_SET), 0, 0, GetGossipItemName(player, ITEM_SET_INSERT_NAME), cost * GetSetCostModifier() + GetSetCopperCost(), true);

    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetGossipItemName(player, ITEM_UPDATE_MENU), sender, action);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetGossipItemName(player, ITEM_BACK), EQUIPMENT_SLOT_END + 4, 0);
    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
}

void Transmogrification::GossipTransmogrify(Player* player, Creature* creature, uint32 const& action, uint32 const& sender)
{
    // sender = slot, action = display
    Transmogrify(player, MAKE_NEW_GUID(action, 0, HIGHGUID_ITEM), sender);
}

void Transmogrification::SendNotification(Player* player, uint8 stringID)
{
    auto session = player->GetSession();
    if (!session)
        return;

    uint8 localeIndex = static_cast<uint8>(session->GetSessionDbLocaleIndex());

    session->SendNotification("%s", sModulesLocale->GetModuleString(MODULE_NAME, stringID, localeIndex).value().c_str());
}
