/*
 * Copyright (C) since 2020 Andrei Guluaev (Winfidonarleyan/Kargatum) https://github.com/Winfidonarleyan
 * Licence MIT https://opensource.org/MIT
 */

#include "Log.h"
#include "ScriptMgr.h"
#include "GameConfig.h"
#include "Chat.h"
#include "Player.h"
#include "Channel.h"
#include "AccountMgr.h"

class FactionsIconsChannel_Player : public PlayerScript
{
public:
    FactionsIconsChannel_Player() : PlayerScript("FactionsIconsChannel_Player") { }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Channel* channel) override
    {
        if (!player || !channel)
            return;

        if (!CONF_GET_BOOL("ChannelIconFaction.Enable"))
            return;

        if (CONF_GET_BOOL("ChannelIconFaction.OnlyLFG") && !channel->IsLFG())
            return;

        if (!CONF_GET_BOOL("ChannelIconFaction.GM") && !AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity()))
            return;

        std::string iconHorge = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18:-3:-3|t";
        std::string iconAlliance = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18:-3:-3|t";

        std::stringstream ssMsg;
        ssMsg << ((player->GetTeamId() == TEAM_HORDE) ? iconHorge : iconAlliance) << msg;
        msg = ssMsg.str();
    }
};

class FactionsIconsChannel_World : public WorldScript
{
public:
    FactionsIconsChannel_World() : WorldScript("FactionsIconsChannel_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddBoolConfig("ChannelIconFaction.Enable");
        sGameConfig->AddBoolConfig("ChannelIconFaction.OnlyLFG");
        sGameConfig->AddBoolConfig("ChannelIconFaction.GM");
    }
};

// Group all custom scripts
void AddSC_FactionsIconsChannel()
{
    new FactionsIconsChannel_Player();
    new FactionsIconsChannel_World();
}
