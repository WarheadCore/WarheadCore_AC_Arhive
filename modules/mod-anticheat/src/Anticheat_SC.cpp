/*
 * Copyright (C) since 2020 Andrei Guluaev (Winfidonarleyan/Kargatum) https://github.com/Winfidonarleyan
 * Licence MIT https://opensource.org/MIT
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
