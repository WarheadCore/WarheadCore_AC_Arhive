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
#include "AccountMgr.h"

class GMChatColor_Player : public PlayerScript
{
public:
    GMChatColor_Player() : PlayerScript("GMChatColor_Player") { }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg) override
    {
        SetColorMessage(player, msg);
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Player* /*receiver*/) override
    {
        SetColorMessage(player, msg);
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Group* /*group*/) override
    {
        SetColorMessage(player, msg);
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Guild* /*guild*/) override
    {
        SetColorMessage(player, msg);
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Channel* /*channel*/) override
    {
        SetColorMessage(player, msg);
    }

private:
    void SetColorMessage(Player* player, std::string& Message)
    {
        if (!CONF_GET_BOOL("GMChatColor.Enable"))
            return;

        if (AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity()) || !player->isGMChat() || Message.empty())
            return;

        uint8 gmLevel = player->GetSession()->GetSecurity();

        if (gmLevel > SEC_ADMINISTRATOR)
            return;

        Message = CONF_GET_STR("GMChatColor.Level." + std::to_string(gmLevel)) + Message;
    };
};

class GMChatColor_World : public WorldScript
{
public:
    GMChatColor_World() : WorldScript("GMChatColor_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddOption<bool>("GMChatColor.Enable");
        sGameConfig->AddOption<std::string>("GMChatColor.Level.1", "|cff0000ff");
        sGameConfig->AddOption<std::string>("GMChatColor.Level.2", "|cff0000ff");
        sGameConfig->AddOption<std::string>("GMChatColor.Level.3", "|cffff0000");
    }
};

// Group all custom scripts
void AddSC_GMChatColor()
{
    new GMChatColor_Player();
    new GMChatColor_World();
}
