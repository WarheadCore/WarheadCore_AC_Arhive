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

class DuelReset_Player : public PlayerScript
{
public:
    DuelReset_Player() : PlayerScript("DuelReset_Player") { }

    // Called when a duel starts (after 3s countdown)
    void OnDuelStart(Player* player1, Player* player2) override
    {
        // Cooldowns reset
        if (CONF_GET_BOOL("DuelResetCooldowns"))
        {
            // Temporary basic cooldown reset
            player1->RemoveArenaSpellCooldowns();
            player2->RemoveArenaSpellCooldowns();
        }

        // Health and mana reset
        if (CONF_GET_BOOL("DuelResetHealthMana"))
        {
            player1->SaveHealthBeforeDuel();
            player1->SetHealth(player1->GetMaxHealth());

            player2->SaveHealthBeforeDuel();
            player2->SetHealth(player2->GetMaxHealth());

            // check if player1 class uses mana
            if (player1->getPowerType() == POWER_MANA || player1->getClass() == CLASS_DRUID)
            {
                player1->SaveManaBeforeDuel();
                player1->SetPower(POWER_MANA, player1->GetMaxPower(POWER_MANA));
            }

            // check if player2 class uses mana
            if (player2->getPowerType() == POWER_MANA || player2->getClass() == CLASS_DRUID)
            {
                player2->SaveManaBeforeDuel();
                player2->SetPower(POWER_MANA, player2->GetMaxPower(POWER_MANA));
            }
        }
    }

    // Called when a duel ends
    void OnDuelEnd(Player* winner, Player* loser, DuelCompleteType type) override
    {
        // do not reset anything if DUEL_INTERRUPTED or DUEL_FLED
        if (type != DUEL_WON)
            return;

        // Health and mana restore
        if (!CONF_GET_BOOL("DuelResetHealthMana"))
            return;

        winner->RestoreHealthAfterDuel();
        loser->RestoreHealthAfterDuel();

        // check if player1 class uses mana
        if (winner->getPowerType() == POWER_MANA || winner->getClass() == CLASS_DRUID)
            winner->RestoreManaAfterDuel();

        // check if player2 class uses mana
        if (loser->getPowerType() == POWER_MANA || loser->getClass() == CLASS_DRUID)
            loser->RestoreManaAfterDuel();
    }
};

class DuelReset_World : public WorldScript
{
public:
    DuelReset_World() : WorldScript("DuelReset_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddOption<bool>("DuelResetCooldowns");
        sGameConfig->AddOption<bool>("DuelResetHealthMana");
    }
};

// Group all custom scripts
void AddSC_DuelReset()
{
    new DuelReset_Player();
    new DuelReset_World();
}
