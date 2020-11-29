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

#ifndef _UNBIND_INSTANCE_H_
#define _UNBIND_INSTANCE_H_

#include "Common.h"
#include "Player.h"
#include "Creature.h"
#include <unordered_map>

constexpr uint32 GOSSIP_SENDER_DIFFICULTY = 10;
constexpr uint32 GOSSIP_ACTION_INFO_MAIN_MENU = 1000;
constexpr uint32 GOSSIP_ACTION_INFO_DUNGEON_HEROIC = GOSSIP_ACTION_INFO_MAIN_MENU + 1;
constexpr uint32 GOSSIP_ACTION_INFO_RAID_10_NORMAL = GOSSIP_ACTION_INFO_DUNGEON_HEROIC + 1;
constexpr uint32 GOSSIP_ACTION_INFO_RAID_25_NORMAL = GOSSIP_ACTION_INFO_RAID_10_NORMAL + 1;
constexpr uint32 GOSSIP_ACTION_INFO_RAID_10_HEROIC = GOSSIP_ACTION_INFO_RAID_25_NORMAL + 1;
constexpr uint32 GOSSIP_ACTION_INFO_RAID_25_HEROIC = GOSSIP_ACTION_INFO_RAID_10_HEROIC + 1;

class UnbindInstance
{
public:
    static UnbindInstance* instance();

    void Init();
    void LoadCostData();

    void SendGossipHello(Player* player, Creature* creature);
    void SendBindList(Player* player, Creature* creature, uint8 diff, bool isRaid = true);
    void BindInfo(Player* player, Creature* creature, uint32 sender, uint32 action);
    void Unbind(Player* player, Creature* creature, uint32 sender, uint32 action, uint32 itemID);
    void SaveLogUnbind(Player* player, uint32 mapID, uint8 diff, uint32 itemID, bool isRaid = true);

private:
    struct UnbindCost
    {
        uint32 ItemID;
        uint32 CountForDungeonHeroic;
        uint32 CountForRaid10Normal;
        uint32 CountForRaid25Normal;
        uint32 CountForRaid10Heroic;
        uint32 CountForRaid25Heroic;
    };

    std::unordered_map<uint32 /*itemid*/, UnbindCost> _costStore;
};

#define sUI UnbindInstance::instance()

#endif /* _UNBIND_INSTANCE_H_ */
