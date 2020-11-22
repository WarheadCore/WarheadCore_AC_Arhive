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
#include "GameTime.h"
#include "ModuleLocale.h"

enum StringLocales : uint8
{
    PIAL_LOCALE_MSG_PREFIX = 1,
    PIAL_LOCALE_MSG_HI,
    PIAL_LOCALE_MSG_GM_LEVEL,
    PIAL_LOCALE_MSG_IP,
    PIAL_LOCALE_MSG_ONLINE,
    PIAL_LOCALE_MSG_UPTIME,

    PIAL_LOCALE_MAX
};

#define MODULE_NAME "mod-player-info-at-login"

class PlayerInfoAtLogin_Player : public PlayerScript
{
public:
    PlayerInfoAtLogin_Player() : PlayerScript("PlayerInfoAtLogin_Player") {}

    void OnLogin(Player* player) override
    {
        if (!CONF_GET_BOOL("PlayerInfoAtLogin.Enable"))
            return;

        uint8 accountLevel = static_cast<uint8>(player->GetSession()->GetSecurity());

        // #1. Prefix
        sModuleLocale->SendPlayerMessage(player, MODULE_NAME, PIAL_LOCALE_MSG_PREFIX);

        // #2. Welcome msg
        sModuleLocale->SendPlayerMessage(player, MODULE_NAME, PIAL_LOCALE_MSG_HI, player->GetName().c_str());

        // #3. Account level if > 0
        if (accountLevel)
            sModuleLocale->SendPlayerMessage(player, MODULE_NAME, PIAL_LOCALE_MSG_GM_LEVEL, accountLevel);

        // #4. IP address
        sModuleLocale->SendPlayerMessage(player, MODULE_NAME, PIAL_LOCALE_MSG_IP, player->GetSession()->GetRemoteAddress().c_str());

        // #5. World online
        sModuleLocale->SendPlayerMessage(player, MODULE_NAME, PIAL_LOCALE_MSG_ONLINE, sWorld->GetPlayerCount(), sWorld->GetMaxActiveSessionCount());

        // #6. World uptime
        sModuleLocale->SendPlayerMessage(player, MODULE_NAME, PIAL_LOCALE_MSG_UPTIME, secsToTimeString(GameTime::GetUptime()).c_str());

        // #7. Prefix again
        sModuleLocale->SendPlayerMessage(player, MODULE_NAME, PIAL_LOCALE_MSG_PREFIX);
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

    void OnStartup() override
    {
        sModuleLocale->CheckStrings(MODULE_NAME, PIAL_LOCALE_MAX);
    }
};

// Group all custom scripts
void AddSC_PlayerInfoAtLogin()
{
    new PlayerInfoAtLogin_Player();
    new PlayerInfoAtLogin_World();
}
