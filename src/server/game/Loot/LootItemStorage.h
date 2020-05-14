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

#ifndef ACORE_LOOTITEMSTORAGE_H
#define ACORE_LOOTITEMSTORAGE_H

#include "Common.h"
#include "LootMgr.h"
#include "Item.h"
#include <map>
#include <list>

struct StoredLootItem
{
    StoredLootItem(uint32 i, uint32 c, int32 ri, uint32 rs) : 
        itemid(i), count(c), randomPropertyId(ri), randomSuffix(rs) { }

    // If itemid == 0 - money amount is stored in count value
    uint32 itemid;
    uint32 count;
    int32 randomPropertyId;
    uint32 randomSuffix;
};

typedef std::list<StoredLootItem> StoredLootItemList;
typedef std::unordered_map<uint32, StoredLootItemList> LootItemContainer;

class LootItemStorage
{
    private:
        LootItemStorage();
        ~LootItemStorage();

    public:
        static LootItemStorage* instance();

        void LoadStorageFromDB();
        void RemoveEntryFromDB(uint32 containerId, uint32 itemid, uint32 count);

        void AddNewStoredLoot(Loot* loot, Player* player);
        bool LoadStoredLoot(Item* item);

        void RemoveStoredLootItem(uint32 containerId, uint32 itemid, uint32 count, Loot* loot);
        void RemoveStoredLootMoney(uint32 containerId, Loot* loot);
        void RemoveStoredLoot(uint32 containerId);

    private:
        LootItemContainer lootItemStore;
};

#define sLootItemStorage LootItemStorage::instance()

#endif
