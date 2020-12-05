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
#include "GameTime.h"
#include "ModuleLocale.h"
#include "MuteManager.h"
#include <vector>

enum AntiADChannelsType : uint8
{
    ANTIAD_CHANNEL_SAY,
    ANTIAD_CHANNEL_WHISPER  = 1,
    ANTIAD_CHANNEL_GROUP    = 2,
    ANTIAD_CHANNEL_GUILD    = 4,
    ANTIAD_CHANNEL_CHANNEL  = 8,
};

enum StringLocales : uint8
{
    ANTIAD_LOCALE_SEND_GM_TEXT = 1,
    ANTIAD_LOCALE_SEND_GM_TEXT_FULL,
    ANTIAD_LOCALE_SEND_SELF,

    ANTIAD_LOCALE_MAX
};

#define MODULE_NAME "mod-anti-ad"

class AntiAD
{
public:
    static AntiAD* instance()
    {
        static AntiAD instance;
        return &instance;
    }

    void Init()
    {
        sModuleLocale->CheckStrings(MODULE_NAME, ANTIAD_LOCALE_MAX);

        LoadDataFromDB();
    }

    void LoadDataFromDB()
    {
        uint32 oldMSTime = getMSTime();

        messages.clear();

        LOG_INFO("module.antiad", "Loading forbidden words...");

        QueryResult result = WorldDatabase.PQuery("SELECT message FROM `anti_ad_messages`");
        if (!result)
        {
            LOG_INFO("module.antiad", ">> Loading 0 word. DB table `anti_ad_messages` is empty.");
            LOG_INFO("module.antiad", "");
            return;
        }

        do
        {
            std::string message = result->Fetch()->GetString();
            messages.push_back(message);

        } while (result->NextRow());

        LOG_INFO("module.antiad", ">> Loaded forbidden words %u in %u ms", static_cast<uint32>(messages.size()), GetMSTimeDiffToNow(oldMSTime));
        LOG_INFO("module.antiad", "");
    }

    bool IsNeedCheckChannel(uint8 channelType)
    {
        if (!CONF_GET_BOOL("AntiAD.Enable"))
            return false;

        return (CONF_GET_INT("AntiAD.CheckChannels") & channelType) ? true : false;
    }

    bool IsBadMessage(std::string& msg)
    {
        if (messages.empty())
            return false;

        msg.erase(remove_if(msg.begin(), msg.end(), ::isspace), msg.end());
        std::transform(msg.begin(), msg.end(), msg.begin(), ::tolower);

        for (auto const& itr : messages)
        {
            if (msg.find(itr) != std::string::npos)
            {
                msg = itr;
                return true;
            }
        }

        return false;
    }

private:
    std::vector<std::string> messages;
};

#define sAD AntiAD::instance()

class AntiAD_Player : public PlayerScript
{
public:
    AntiAD_Player() : PlayerScript("AntiAD_Player") { }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg) override
    {
        if (!sAD->IsNeedCheckChannel(ANTIAD_CHANNEL_SAY))
            return;

        CheckMessage(player, msg);
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Player* /*receiver*/) override
    {
        if (!sAD->IsNeedCheckChannel(ANTIAD_CHANNEL_WHISPER))
            return;

        CheckMessage(player, msg);
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Group* /*group*/) override
    {
        if (!sAD->IsNeedCheckChannel(ANTIAD_CHANNEL_GROUP))
            return;

        CheckMessage(player, msg);
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Guild* /*guild*/) override
    {
        if (!sAD->IsNeedCheckChannel(ANTIAD_CHANNEL_GUILD))
            return;

        CheckMessage(player, msg);
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Channel* /*channel*/) override
    {
        if (!sAD->IsNeedCheckChannel(ANTIAD_CHANNEL_CHANNEL))
            return;

        CheckMessage(player, msg);
    }

private:
    void Mute(Player* player)
    {
        if (!(CONF_GET_BOOL("AntiAD.MuteGM.Enable") && !AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity())))
            return;

        uint32 muteTime = CONF_GET_INT("AntiAD.Mute.Time");

        sMute->MutePlayer(player->GetName(), muteTime, "Console", "Advertisment");

        if (CONF_GET_BOOL("AntiAD.SelfMessage.Enable"))
            sModuleLocale->SendGlobalMessage(true, MODULE_NAME, StringLocales::ANTIAD_LOCALE_SEND_SELF, muteTime);
    }

    void SendGMTexts(Player* player, std::string ADMessage, std::string FullMessage)
    {
        uint8 loc_idx = uint8(player->GetSession()->GetSessionDbLocaleIndex());
        std::string NameLink = ChatHandler(player->GetSession()).GetNameLink(player);

        if (CONF_GET_BOOL("AntiAD.MessageGMsInWorld.Enable"))
            sModuleLocale->SendGlobalMessage(true, MODULE_NAME, StringLocales::ANTIAD_LOCALE_SEND_GM_TEXT, NameLink.c_str(), ADMessage.c_str());

        if (CONF_GET_BOOL("AntiAD.FullMessageGMsInWorld.Enable"))
            sModuleLocale->SendGlobalMessage(true, MODULE_NAME, StringLocales::ANTIAD_LOCALE_SEND_GM_TEXT_FULL, NameLink.c_str(), FullMessage.c_str());
    }

    void CheckMessage(Player* player, std::string& msg)
    {
        if (!CONF_GET_BOOL("AntiAD.Enable"))
            return;

        std::string CheckMsg = msg;
        std::string FullMessage = msg;

        if (sAD->IsBadMessage(CheckMsg))
        {
            SendGMTexts(player, CheckMsg, FullMessage);
            msg = "";
            Mute(player);
        }
    };
};

class AntiAD_World : public WorldScript
{
public:
    AntiAD_World() : WorldScript("AntiAD_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddOption<bool>("AntiAD.Enable");
        sGameConfig->AddOption<bool>("AntiAD.MessageGMsInWorld.Enable");
        sGameConfig->AddOption<bool>("AntiAD.FullMessageGMsInWorld.Enable");
        sGameConfig->AddOption<bool>("AntiAD.SelfMessage.Enable");
        sGameConfig->AddOption<bool>("AntiAD.Mute.Enable");
        sGameConfig->AddOption<bool>("AntiAD.MuteGM.Enable");

        sGameConfig->AddOption<int32>("AntiAD.Mute.Time", 5);
        sGameConfig->AddOption<int32>("AntiAD.CheckChannels", 8);
    }

    void OnStartup() override
    {
        if (!CONF_GET_BOOL("AntiAD.Enable"))
            return;

        sAD->Init();
    }
};

// Group all custom scripts
void AddSC_AntiAD()
{
    new AntiAD_Player();
    new AntiAD_World();
}
