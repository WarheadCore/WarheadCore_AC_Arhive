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

#include "Log.h"
#include "ScriptMgr.h"
#include "GameConfig.h"
#include "Chat.h"
#include "Player.h"
#include "ScriptedGossip.h"

class PlayerCanGetExp_Player : public PlayerScript
{
public:
    PlayerCanGetExp_Player() : PlayerScript("PlayerCanGetExp_Player") { }

    void OnGiveXP(Player* player, uint32& amount, Unit* victim)
    {
        if (CONF_GET_BOOL("PCGE.Enable"))
            return;

        // For only kill creature
        if (!victim)
            return;

        // Get levels
        uint8 playerLevel = 0, creatureLevel = 0;
        playerLevel = player->getLevel();
        creatureLevel = victim->getLevel();

        // If player level same or less creature level - skip
        if (playerLevel >= creatureLevel)
            return;

        // Get diff levels
        int32 diff = creatureLevel - playerLevel;

        // If If player level less than creature level - set exp 0
        if (diff > CONF_GET_INT("PCGE.MaxLevelDiff"))
            amount = 0;
    }
};

class PlayerCanGetExp_World : public WorldScript
{
public:
    PlayerCanGetExp_World() : WorldScript("PlayerCanGetExp_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddOption<bool>("PCGE.Enable");
        sGameConfig->AddOption<int32>("PCGE.MaxLevelDiff", 10);
    }
};

// Group all custom scripts
void AddSC_PlayerCanGetExp()
{
    new PlayerCanGetExp_Player();
    new PlayerCanGetExp_World();
}
