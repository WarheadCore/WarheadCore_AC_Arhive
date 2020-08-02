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
#include "Player.h"
#include "AccountMgr.h"

class AutoRevive_Player : public PlayerScript
{
public:
    AutoRevive_Player() : PlayerScript("AutoRevive_Player") {}

    void OnBeforeUpdate(Player* player, uint32 diff) override
    {
        if (!CONF_GET_BOOL("AutoRevive.Enable"))
            return;

        if (AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity()))
            return;

        uint32 NowHealth = player->GetHealth();
        uint32 TimeCheck = 5000;
        uint32 configZoneID = CONF_GET_INT("AutoRevive.ZoneID");
        uint32 zoneID = player->GetZoneId();

        if (TimeCheck_AutoRevive < diff)
        {
            if (NowHealth <= 1)
            {
                if (configZoneID)
                {
                    if (configZoneID == zoneID)
                    {
                        player->ResurrectPlayer(1.0f);
                        player->SaveToDB(false, false);
                    }
                }
                else
                {
                    player->ResurrectPlayer(1.0f);
                    player->SaveToDB(false, false);
                }
            }

            TimeCheck_AutoRevive = TimeCheck;
        }
        else
            TimeCheck_AutoRevive -= diff;
    }

private:
    uint32 TimeCheck_AutoRevive = 0;
};

class AutoRevive_World : public WorldScript
{
public:
    AutoRevive_World() : WorldScript("AutoRevive_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddBoolConfig("AutoRevive.Enable");
        sGameConfig->AddIntConfig("AutoRevive.ZoneID");
    }
};

// Group all custom scripts
void AddSC_AutoRevive()
{
    new AutoRevive_Player();
    new AutoRevive_World();
}
