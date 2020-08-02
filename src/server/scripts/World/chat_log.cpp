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

#include "ScriptMgr.h"
#include "Channel.h"
#include "Guild.h"
#include "Group.h"

class ChatLogScript : public PlayerScript
{
public:
    ChatLogScript() : PlayerScript("ChatLogScript") { }

    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg) override
    {
        switch (type)
        {
        case CHAT_MSG_ADDON:
            LOG_DEBUG("chat.log.addon", "[ADDON] Player %s sends: %s", player->GetName().c_str(), msg.c_str());
            break;
        case CHAT_MSG_SAY:
            LOG_DEBUG("chat.log.say", "[SAY] Player %s says (language %u): %s", player->GetName().c_str(), lang, msg.c_str());
            break;
        case CHAT_MSG_EMOTE:
            LOG_DEBUG("chat.log.emote", "[TEXTEMOTE] Player %s emotes: %s", player->GetName().c_str(), msg.c_str());
            break;
        case CHAT_MSG_YELL:
            LOG_DEBUG("chat.log.yell", "[YELL] Player %s yells (language %u): %s", player->GetName().c_str(), lang, msg.c_str());
            break;
        }
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Player* receiver) override
    {
        if (lang != LANG_ADDON)
            LOG_DEBUG("chat.log.whisper", "[WHISPER] Player %s tells %s: %s",
                player->GetName().c_str(), receiver ? receiver->GetName().c_str() : "<unknown>", msg.c_str());
        else
            LOG_DEBUG("chat.log.addon.whisper", "[ADDON] Player %s tells %s: %s",
                player->GetName().c_str(), receiver ? receiver->GetName().c_str() : "<unknown>", msg.c_str());
    }

    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg, Group* group) override
    {
        //! NOTE:
        //! LANG_ADDON can only be sent by client in "PARTY", "RAID", "GUILD", "BATTLEGROUND", "WHISPER"
        switch (type)
        {
        case CHAT_MSG_PARTY:
            if (lang != LANG_ADDON)
                LOG_DEBUG("chat.log.party", "[PARTY] Player %s tells group with leader %s: %s",
                    player->GetName().c_str(), group ? group->GetLeaderName() : "<unknown>", msg.c_str());
            else
                LOG_DEBUG("chat.log.addon.party", "[ADDON] Player %s tells group with leader %s: %s",
                    player->GetName().c_str(), group ? group->GetLeaderName() : "<unknown>", msg.c_str());
            break;
        case CHAT_MSG_PARTY_LEADER:
            LOG_DEBUG("chat.log.party", "[PARTY] Leader %s tells group: %s", player->GetName().c_str(), msg.c_str());
            break;
        case CHAT_MSG_RAID:
            if (lang != LANG_ADDON)
                LOG_DEBUG("chat.log.raid", "[RAID] Player %s tells raid with leader %s: %s",
                    player->GetName().c_str(), group ? group->GetLeaderName() : "<unknown>", msg.c_str());
            else
                LOG_DEBUG("chat.log.addon.raid", "[ADDON] Player %s tells raid with leader %s: %s",
                    player->GetName().c_str(), group ? group->GetLeaderName() : "<unknown>", msg.c_str());
            break;
        case CHAT_MSG_RAID_LEADER:
            LOG_DEBUG("chat.log.raid", "[RAID] Leader player %s tells raid: %s", player->GetName().c_str(), msg.c_str());
            break;
        case CHAT_MSG_RAID_WARNING:
            LOG_DEBUG("chat.log.raid", "[RAID] Leader player %s warns raid with: %s", player->GetName().c_str(), msg.c_str());
            break;
        case CHAT_MSG_BATTLEGROUND:
            if (lang != LANG_ADDON)
                LOG_DEBUG("chat.log.bg", "[BATTLEGROUND] Player %s tells battleground with leader %s: %s",
                    player->GetName().c_str(), group ? group->GetLeaderName() : "<unknown>", msg.c_str());
            else
                LOG_DEBUG("chat.log.addon.bg", "[ADDON] Player %s tells battleground with leader %s: %s",
                    player->GetName().c_str(), group ? group->GetLeaderName() : "<unknown>", msg.c_str());
            break;
        case CHAT_MSG_BATTLEGROUND_LEADER:
            LOG_DEBUG("chat.log.bg", "[BATTLEGROUND] Leader player %s tells battleground: %s",
                player->GetName().c_str(), msg.c_str());
            break;
        }
    }

    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg, Guild* guild) override
    {
        switch (type)
        {
        case CHAT_MSG_GUILD:
            if (lang != LANG_ADDON)
                LOG_DEBUG("chat.log.guild", "[GUILD] Player %s tells guild %s: %s",
                    player->GetName().c_str(), guild ? guild->GetName().c_str() : "<unknown>", msg.c_str());
            else if (lang == LANG_ADDON)
                LOG_DEBUG("chat.log.addon.guild", "[ADDON] Player %s sends to guild %s: %s",
                    player->GetName().c_str(), guild ? guild->GetName().c_str() : "<unknown>", msg.c_str());
            break;
        case CHAT_MSG_OFFICER:
            LOG_DEBUG("chat.log.guild", "[OFFICER] Player %s tells guild %s officers: %s",
                player->GetName().c_str(), guild ? guild->GetName().c_str() : "<unknown>", msg.c_str());
            break;
        }
    }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Channel* channel) override
    {
        bool isSystem = channel &&
            (channel->HasFlag(CHANNEL_FLAG_TRADE) ||
                channel->HasFlag(CHANNEL_FLAG_GENERAL) ||
                channel->HasFlag(CHANNEL_FLAG_CITY) ||
                channel->HasFlag(CHANNEL_FLAG_LFG));

        if (isSystem)
            LOG_DEBUG("scripts", "[SYSCHAN] Player %s tells channel %s: %s", player->GetName().c_str(), channel->GetName().c_str(), msg.c_str());
        else
        {
            std::string channelName = channel ? channel->GetName() : "<unknown>";
            LOG_DEBUG("chat.log.channel." + channelName, "[CHANNEL] Player %s tells channel %s: %s", player->GetName().c_str(), channelName.c_str(), msg.c_str());
        }
    }
};

void AddSC_chat_log()
{
    new ChatLogScript();
}
