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

#include "UnbindInstance.h"
#include "Log.h"
#include "ScriptMgr.h"
#include "GameConfig.h"
#include "Chat.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "StringConvert.h"

class UnbindInstance_Creature : public CreatureScript
{
public:
    UnbindInstance_Creature() : CreatureScript("UnbindInstance_Creature") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (!CONF_GET_BOOL("UnbindInsance.Enable"))
            return true;

        sUI->SendGossipHello(player, creature);
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        ClearGossipMenuFor(player);

        if (sender >= GOSSIP_SENDER_DIFFICULTY)
        {
            sUI->BindInfo(player, creature, sender, action);
            return true;
        }

        switch (action)
        {
            case GOSSIP_ACTION_INFO_MAIN_MENU:
                sUI->SendGossipHello(player, creature);
                break;
            case GOSSIP_ACTION_INFO_DUNGEON_HEROIC:
                sUI->SendBindList(player, creature, 1, false);
                break;
            case GOSSIP_ACTION_INFO_RAID_10_NORMAL:
                sUI->SendBindList(player, creature, 0);
                break;
            case GOSSIP_ACTION_INFO_RAID_25_NORMAL:
                sUI->SendBindList(player, creature, 1);
                break;
            case GOSSIP_ACTION_INFO_RAID_10_HEROIC:
                sUI->SendBindList(player, creature, 2);
                break;
            case GOSSIP_ACTION_INFO_RAID_25_HEROIC:
                sUI->SendBindList(player, creature, 3);
                break;
            default:
                break;
        }

        return true;
    }

    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, char const* code) override
    {
        ClearGossipMenuFor(player);
        sUI->Unbind(player, creature, sender, action, Warhead::StringTo<uint32>(code).value_or(0));

        return true;
    }
};

class UnbindInstance_World : public WorldScript
{
public:
    UnbindInstance_World() : WorldScript("UnbindInstance_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddOption<bool>("UnbindInsance.Enable");
    }

    void OnStartup() override
    {
        sUI->Init();
    }
};

// Group all custom scripts
void AddSC_UnbindInstance()
{
    new UnbindInstance_Creature();
    new UnbindInstance_World();
}
