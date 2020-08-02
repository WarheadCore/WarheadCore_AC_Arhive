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

#include "Anticheat.h"
#include "Log.h"
#include "ScriptMgr.h"
#include "GameConfig.h"
#include "Chat.h"
#include "Player.h"

class Anticheat_Player : public PlayerScript
{
public:
    Anticheat_Player() : PlayerScript("Anticheat_Player") { }

    void OnLogout(Player* player) override
    {
        sAnticheat->HandlePlayerLogout(player);
    }

    void OnLogin(Player* player) override
    {
        sAnticheat->HandlePlayerLogin(player);        
    }
};

class Anticheat_Movement : public MovementHandlerScript
{
public:
    Anticheat_Movement() : MovementHandlerScript("Anticheat_Movement") { }

    void OnPlayerMove(Player* player, MovementInfo mi, uint32 opcode) override
    {
        sAnticheat->StartHackDetection(player, mi, opcode);
    }
};

class Anticheat_World : public WorldScript
{
public:
    Anticheat_World() : WorldScript("Anticheat_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddBoolConfig("AC.Enable");
        sGameConfig->AddBoolConfig("AC.Info.Enable");
        sGameConfig->AddBoolConfig("AC.Detect.FlyHack.Enable");
        sGameConfig->AddBoolConfig("AC.Detect.WaterWalkHack.Enable");
        sGameConfig->AddBoolConfig("AC.Detect.JumpHack.Enable");
        sGameConfig->AddBoolConfig("AC.Detect.TelePlaneHack.Enable");
        sGameConfig->AddBoolConfig("AC.Detect.SpeedHack.Enable");
        sGameConfig->AddBoolConfig("AC.Detect.ClimbHack.Enable");
    }
};

// Group all custom scripts
void AddSC_Anticheat()
{
    new Anticheat_Player();
    new Anticheat_Movement();
    new Anticheat_World();
}
