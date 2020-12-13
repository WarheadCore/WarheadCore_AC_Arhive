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
        sGameConfig->AddOption<bool>("ChannelIconFaction.Enable");
        sGameConfig->AddOption<bool>("ChannelIconFaction.OnlyLFG");
        sGameConfig->AddOption<bool>("ChannelIconFaction.GM");
    }
};

// Group all custom scripts
void AddSC_FactionsIconsChannel()
{
    new FactionsIconsChannel_Player();
    new FactionsIconsChannel_World();
}
