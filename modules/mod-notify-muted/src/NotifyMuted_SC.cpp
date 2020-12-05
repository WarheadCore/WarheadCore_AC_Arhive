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
#include "MuteManager.h"
#include "ModuleLocale.h"

enum StringLocales : uint8
{
    NM_LOCALE_MESSAGE = 1,

    NM_LOCALE_MAX
};

#define MODULE_NAME "mod-notify-muted"

class NotifyMuted_Player : public PlayerScript
{
public:
    NotifyMuted_Player() : PlayerScript("NotifyMuted_Player") {}

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& /*msg*/, Player* receiver) override
    {
        if (!CONF_GET_BOOL("NotCanSpeakMsg.Enable"))
            return;

        if (receiver->CanSpeak())
            return;

        uint64 MuteTime = sMute->GetMuteTime(receiver->GetSession()->GetAccountId());
        if (!MuteTime)
            return;

        std::string muteTimeStr = secsToTimeString(MuteTime - GameTime::GetGameTime(), true);
        std::string nameLink = ChatHandler(receiver->GetSession()).playerLink(receiver->GetName());

        sModuleLocale->SendPlayerMessage(player, MODULE_NAME, NM_LOCALE_MESSAGE, nameLink.c_str(), muteTimeStr.c_str());
    }
};

class NotifyMuted_World : public WorldScript
{
public:
    NotifyMuted_World() : WorldScript("NotifyMuted_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddOption<bool>("NotCanSpeakMsg.Enable");
    }

    void OnStartup() override
    {
        sModuleLocale->CheckStrings(MODULE_NAME, NM_LOCALE_MAX);
    }
};

// Group all custom scripts
void AddSC_NotifyMuted()
{
    new NotifyMuted_Player();
    new NotifyMuted_World();
}
