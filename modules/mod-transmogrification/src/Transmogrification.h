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

#ifndef DEF_TRANSMOGRIFICATION_H
#define DEF_TRANSMOGRIFICATION_H

#include "Common.h"
#include "Player.h"
#include "Item.h"
#include "WorldSession.h"
#include <set>
#include <unordered_map>
#include <map>

#define MAX_OPTIONS 25 // do not alter

enum TransmogrificationGossipItem
{
    ITEM_HOW_WORKS,
    ITEM_MANAGE_SETS,
    ITEM_REMOVE_ALL_TRANSMOG,
    ITEM_REMOVE_SINGLE_TRANSMOG,
    ITEM_UPDATE_MENU,
    ITEM_BACK,
    ITEM_REMOVE_ALL_TRANSMOG_Q,
    ITEM_REMOVE_SINGLE_TRANSMOG_Q,
    ITEM_SAVE_SET,
    ITEM_ITEM_BIND_Q,
    ITEM_HOW_SET_WORKS,
    ITEM_SET_USE,
    ITEM_SET_DELETE,
    ITEM_SET_DELETE_Q,
    ITEM_SET_INSERT_NAME
};

class Transmogrification
{
public:
    static Transmogrification* instance();

    // Config options
    void Init();
    void LoadConfig(bool reload);

    bool CanTransmogSlot(uint8 slot) const;

    uint32 GetSpecialPrice(ItemTemplate const* proto) const;

    void DeleteFakeFromDB(uint64 itemGUID, SQLTransaction* trans = nullptr);

    bool IsAllowed(uint32 entry) const;
    bool IsNotAllowed(uint32 entry) const;
    bool IsAllowedQuality(uint32 quality) const;
    bool IsRangedWeapon(uint32 Class, uint32 SubClass) const;

    void PresetTransmog(Player* player, Item* itemTransmogrified, uint32 fakeEntry, uint8 slot);

    void LoadPlayerSets(uint64 pGUID);
    void UnloadPlayerSets(uint64 pGUID);

    std::string GetItemIcon(uint32 entry, uint32 width, uint32 height, int x, int y) const;
    std::string GetSlotIcon(uint8 slot, uint32 width, uint32 height, int x, int y) const;
    std::string const GetSlotName(Player* player, uint8 slot) const;
    std::string GetItemLink(Item* item, WorldSession* session) const;
    std::string GetItemLink(uint32 entry, WorldSession* session) const;
    uint32 GetFakeEntry(uint64 itemGUID) const;
    void UpdateItem(Player* player, Item* item) const;
    void DeleteFakeEntry(Player* player, uint8 slot, Item* itemTransmogrified, SQLTransaction* trans = nullptr);
    void SetFakeEntry(Player* player, uint32 newEntry, uint8 slot, Item* itemTransmogrified);

    void Transmogrify(Player* player, uint64 itemGUID, uint8 slot, bool no_cost = false);
    void SendNotification(Player* player, uint8 stringID);
    bool CanTransmogrifyItemWithItem(Player* player, ItemTemplate const* destination, ItemTemplate const* source) const;
    bool SuitableForTransmogrification(Player* player, ItemTemplate const* proto) const;

    void ClearPlayerAtLogout(Player* player);
    void LoadPlayerAtLogin(Player* player);
    std::string const GetGossipIcon(uint8 slot, Player* player);
    std::string const GetGossipItemName(Player* player, TransmogrificationGossipItem gossipItem);

    bool CanSavePresets(Player* player);
    void SavePreset(Player* player, Creature* creature, std::string const& name);

    void GossipShowTransmogItems(Player* player, Creature* creature, uint8 slot);
    void GossipRemoveAllTransmogrifications(Player* player);
    void GossipRemoveSingleTransmogrifications(Player* player, uint32 const& action);
    void GossipShowPresetsMenu(Player* player, Creature* creature);
    void GossipUsePreset(Player* player, Creature* creature, uint32 const& action);
    void GossipViewPreset(Player* player, Creature* creature, uint32 const& action, uint32 const& sender);
    void GossipDeletePreset(Player* player, Creature* creature, uint32 const& action);
    void GossipSavePreset(Player* player, Creature* creature, uint32 const& action, uint32 const& sender);
    void GossipTransmogrify(Player* player, Creature* creature, uint32 const& action, uint32 const& sender);

private:
    typedef std::unordered_map<uint64, uint64> TransmogrificationDataContainer;
    typedef std::unordered_map<uint64, TransmogrificationDataContainer> TransmogrificationMapContainer;
    typedef std::map<uint8, uint32> SlotsContainer;
    typedef std::map<uint8, SlotsContainer> PresetsContainer;
    typedef std::unordered_map<uint64, PresetsContainer> PresetsDataMapContainer;
    typedef std::map<uint8, std::string> PresetsIdMapContainer;
    typedef std::unordered_map<uint64, PresetsIdMapContainer> PresetsNameMapContainer;

    PresetsNameMapContainer _presetByName; // _presetByName[pGUID][presetID] = presetName
    PresetsDataMapContainer _presetById; // _presetById[pGUID][presetID][slot] = entry
    TransmogrificationMapContainer _mapStore;
    TransmogrificationDataContainer _dataMapStore;

    // Use IsAllowed() and IsNotAllowed()
    // these are thread unsafe, but assumed to be static data so it should be safe
    std::set<uint32> Allowed;
    std::set<uint32> NotAllowed;
};

#define sTransmog Transmogrification::instance()

#endif
