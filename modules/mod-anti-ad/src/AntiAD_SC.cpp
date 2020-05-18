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
#include "GameLocale.h"
#include <vector>

enum LocaleStrings
{
    ANTIAD_SEND_GM_TEXT = 1,
    ANTIAD_SEND_GM_TEXT_FULL,
    ANTIAD_SEND_SELF,
};

class AntiAD
{
public:
    static AntiAD* instance()
    {
        static AntiAD instance;
        return &instance;
    }

    void LoadDataFromDB()
    {
        uint32 oldMSTime = getMSTime();

        messages.clear();

        LOG_INFO("module", "Loading forbidden words...");

        QueryResult result = WorldDatabase.PQuery("SELECT message FROM `anti_ad_messages`");
        if (!result)
        {
            sLog->outString(">> Loading 0 word. DB table `anti_ad_messages` is empty.");
            sLog->outString();
            return;
        }

        do
        {
            std::string message = result->Fetch()->GetString();
            messages.push_back(message);

        } while (result->NextRow());

        sLog->outString(">> Loaded forbidden words %u in %u ms", static_cast<uint32>(messages.size()), GetMSTimeDiffToNow(oldMSTime));
        sLog->outString();
    }

    bool IsBadMessage(std::string& msg)
    {
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
        CheckMessage(player, msg);
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Player* /*receiver*/) override
    {
        CheckMessage(player, msg);
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Group* /*group*/) override
    {
        CheckMessage(player, msg);
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Guild* /*guild*/) override
    {
        CheckMessage(player, msg);
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Channel* /*channel*/) override
    {
        CheckMessage(player, msg);
    }
    
private:
    void Mute(Player* player)
    {
        if (!(CONF_GET_BOOL("AntiAD.MuteGM.Enable") && !AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity())))
            return;

        uint32 muteTime = CONF_GET_INT("AntiAD.Mute.Time");

        player->GetSession()->m_muteTime = time(nullptr) + muteTime * MINUTE;

        if (CONF_GET_BOOL("AntiAD.SelfMessage.Enable"))
            sGameLocale->SendPlayerMessage(player, "mod-anti-ad", ANTIAD_SEND_SELF, muteTime);
    }

    void SendGMTexts(Player* player, std::string ADMessage, std::string FullMessage)
    {
        uint8 loc_idx = uint8(player->GetSession()->GetSessionDbLocaleIndex());
        std::string NameLink = ChatHandler(player->GetSession()).GetNameLink(player);

        if (CONF_GET_BOOL("AntiAD.MessageGMsInWorld.Enable"))
            sGameLocale->SendGlobalMessage("mod-anti-ad", ANTIAD_SEND_GM_TEXT, true, NameLink.c_str(), ADMessage.c_str());

        if (CONF_GET_BOOL("AntiAD.FullMessageGMsInWorld.Enable"))
            sGameLocale->SendGlobalMessage("mod-anti-ad", ANTIAD_SEND_GM_TEXT_FULL, true, NameLink.c_str(), FullMessage.c_str());
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
        sGameConfig->AddBoolConfig("AntiAD.Enable");
        sGameConfig->AddBoolConfig("AntiAD.MessageGMsInWorld.Enable");
        sGameConfig->AddBoolConfig("AntiAD.FullMessageGMsInWorld.Enable");
        sGameConfig->AddBoolConfig("AntiAD.SelfMessage.Enable");
        sGameConfig->AddBoolConfig("AntiAD.Mute.Enable");
        sGameConfig->AddBoolConfig("AntiAD.MuteGM.Enable");

        sGameConfig->AddIntConfig("AntiAD.Mute.Time", 5);
    }

    void OnStartup() override
    {
        if (!CONF_GET_BOOL("AntiAD.Enable"))
            return;
        
        sAD->LoadDataFromDB();
    }
};

// Group all custom scripts
void AddSC_AntiAD()
{
    new AntiAD_Player();
    new AntiAD_World();
}
