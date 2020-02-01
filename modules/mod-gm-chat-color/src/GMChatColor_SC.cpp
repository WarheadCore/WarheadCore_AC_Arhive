/*
 * Copyright (C) since 2020 Andrei Guluaev (Winfidonarleyan/Kargatum) https://github.com/Winfidonarleyan
 * Licence MIT https://opensource.org/MIT
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
        sGameConfig->AddBoolConfig("GMChatColor.Enable");
        sGameConfig->AddStringConfig("GMChatColor.Level.1", "|cff0000ff");
        sGameConfig->AddStringConfig("GMChatColor.Level.2", "|cff0000ff");
        sGameConfig->AddStringConfig("GMChatColor.Level.3", "|cffff0000");
    }
};

// Group all custom scripts
void AddSC_GMChatColor()
{
    new GMChatColor_Player();
    new GMChatColor_World();
}
