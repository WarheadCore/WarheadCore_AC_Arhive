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

namespace lang
{
    enum KargatumStrings
    {
        RECEIVER_NOT_SPEAK = 50003
    };
}

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

        uint64 MuteTime = receiver->GetSession()->m_muteTime;
        if (!MuteTime)
            return;

        std::string MuteTimeStr = secsToTimeString(MuteTime - GameTime::GetGameTime(), true);
        std::string NameLink = ChatHandler(receiver->GetSession()).playerLink(receiver->GetName());

        ChatHandler(player->GetSession()).PSendSysMessage(lang::RECEIVER_NOT_SPEAK, NameLink.c_str(), MuteTimeStr.c_str());
    }
};

class NotifyMuted_World : public WorldScript
{
public:
    NotifyMuted_World() : WorldScript("NotifyMuted_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddBoolConfig("NotCanSpeakMsg.Enable");
    }
};

// Group all custom scripts
void AddSC_NotifyMuted()
{
    new NotifyMuted_Player();
    new NotifyMuted_World();
}
