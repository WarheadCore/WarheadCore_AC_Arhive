/*
 * Copyright (C) since 2020 Andrei Guluaev (Winfidonarleyan/Kargatum) https://github.com/Winfidonarleyan
 * Licence MIT https://opensource.org/MIT
 */

#include "Log.h"
#include "ScriptMgr.h"
#include "GameConfig.h"
#include "Chat.h"
#include "Player.h"
#include "GameTime.h"

class PlayerInfoAtLogin_Player : public PlayerScript
{
public:
    PlayerInfoAtLogin_Player() : PlayerScript("PlayerInfoAtLogin_Player") {}

    void OnLogin(Player* player) override
    {
        if (!CONF_GET_BOOL("PlayerInfoAtLogin.Enable"))
            return;

        ChatHandler handler(player->GetSession());

        std::string NameLink = handler.GetNameLink(player);
        std::string PlayerName = player->GetName();
        uint32 PlayerOnlineCount = sWorld->GetPlayerCount();
        std::string ServerUptime = secsToTimeString(GameTime::GetUptime());
        std::string PlayerIP = player->GetSession()->GetRemoteAddress();
        uint32 GMLevel = player->GetSession()->GetSecurity();
        uint32 connPeak = sWorld->GetMaxActiveSessionCount();

        handler.PSendSysMessage("|cffff0000##############################|r");
        handler.PSendSysMessage("|cffff0000# |cff00ff00Hi,|r %s", PlayerName.c_str());

        if (GMLevel)
            handler.PSendSysMessage("|cffff0000# |cff00ff00You account level:|r %u", GMLevel);

        handler.PSendSysMessage("|cffff0000# |cff00ff00You IP:|r %s", PlayerIP.c_str());
        handler.PSendSysMessage("|cffff0000# |cff00ff00Now|r %u |cff00ff00players online|r |cff00ff00(max:|r %u|cff00ff00)|r", PlayerOnlineCount, connPeak);
        handler.PSendSysMessage("|cffff0000# |cff00ff00Server uptime:|r %s", ServerUptime.c_str());
        handler.PSendSysMessage("|cffff0000##############################|r");
    }
};

class PlayerInfoAtLogin_World : public WorldScript
{
public:
    PlayerInfoAtLogin_World() : WorldScript("PlayerInfoAtLogin_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddBoolConfig("PlayerInfoAtLogin.Enable");
    }
};

// Group all custom scripts
void AddSC_PlayerInfoAtLogin()
{
    new PlayerInfoAtLogin_Player();
    new PlayerInfoAtLogin_World();
}
