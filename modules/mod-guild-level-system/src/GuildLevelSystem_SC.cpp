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

#include "GuildLevelSystem.h"
#include "Log.h"
#include "ScriptMgr.h"
#include "GameConfig.h"
#include "Chat.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "DatabaseEnv.h"

class Kargatum_Guild : public GuildScript
{
public: Kargatum_Guild() : GuildScript("Kargatum_Guild") { }

    void OnCreate(Guild* guild, Player* /*leader*/, const std::string& /*name*/) override
    {
        if (!CONF_GET_BOOL("GLS.Enable"))
            return;

        sGuildLevelSystem->SetFullNameFirstLevel(guild);
    }

    void OnGuildBeforeCreate(Guild* guild) override
    {
        if (!CONF_GET_BOOL("GLS.Enable"))
            return;

        sGuildLevelSystem->AddEmptyGLS(guild);
    }

    void OnDisband(Guild* guild) override
    {
        if (!CONF_GET_BOOL("GLS.Enable"))
            return;

        sGuildLevelSystem->DeleteGLS(guild);
    }

    void OnAddMember(Guild* guild, Player* player, uint8& /*plRank*/) override
    {
        if (!CONF_GET_BOOL("GLS.Enable"))
            return;

        //sGuildLevelSystem->UpdateGuildVisibleLevel(guild->GetId());
        sGuildLevelSystem->RewardSpellGuildMember(guild, player);
    }

    void OnRemoveMember(Guild* guild, Player* player, bool /*isDisbanding*/, bool /*isKicked*/) override
    {
        if (!CONF_GET_BOOL("GLS.Enable"))
            return;

        sGuildLevelSystem->UnRewardSpellGuildMember(guild, player);
    }
};

class Kargatum_Guild_Player : public PlayerScript
{
public: Kargatum_Guild_Player() : PlayerScript("Kargatum_Guild_Player") { }

    void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Guild* guild) override
    {
        if (player->GetRank() > 1)
            return;

        if (!CONF_GET_BOOL("GLS.Enable"))
            return;

        std::string color = "|cffff0000";

        switch (player->GetRank())
        {
        case 0:
            color = CONF_GET_STR("GLS.ColorChat.Rank0");
            break;
        case 1:
            color = CONF_GET_STR("GLS.ColorChat.Rank1");
            break;
        default:
            break;
        }

        if (sGuildLevelSystem->IsAsseptSetting(GUILD_SETTING_FLAG_ADM_CHAT, guild) && player->GetRank() == 0)
            msg = color + msg + "|r";

        if (sGuildLevelSystem->IsAsseptSetting(GUILD_SETTING_FLAG_ASSISTANT_CHAT, guild) && player->GetRank() == 1)
            msg = color + msg + "|r";
    }

    void OnLogin(Player* player) override
    {
        if (!CONF_GET_BOOL("GLS.Enable"))
            return;

        if (!player->GetGuild())
            return;

        sGuildLevelSystem->RewardSpellGuildMember(player->GetGuild(), player);
    }
};

class Kargatum_Guild_Creature : public CreatureScript
{
public: Kargatum_Guild_Creature() : CreatureScript("Kargatum_Guild_Creature") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        ClearGossipMenuFor(player);
        uint32 guild = player->GetGuildId();
        ChatHandler handler(player->GetSession());

        if (!guild)
        {
            CloseGossipMenuFor(player);
            handler.PSendSysMessage("Вы не состоите в гильдии");
            return true;
        }

        if (!CONF_GET_BOOL("GLS.Enable"))
        {
            CloseGossipMenuFor(player);
            handler.PSendSysMessage("Система уровней гильдий выключена");
            return true;
        }

        // |TInterface/ICONS/Spell_Magic_ManaGain:32:32:-18:0|t

        AddGossipItemFor(player, 10, "Информация о гильдии", GOSSIP_SENDER_MAIN, 1);
        AddGossipItemFor(player, 10, "Вклад в гильдию", GOSSIP_SENDER_MAIN, 2);

        if (player->GetRank() == 0)
            AddGossipItemFor(player, 10, "Настройки гильдии", GOSSIP_SENDER_MAIN, 3);

        AddGossipItemFor(player, 10, "Информация о наградах", GOSSIP_SENDER_MAIN, 4);
        AddGossipItemFor(player, 10, "Выход", GOSSIP_SENDER_MAIN, 100);

        SendGossipMenuFor(player, 1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        ClearGossipMenuFor(player);
        uint32 ItemCount = sGuildLevelSystem->GetCountExpItem(player);
        std::string AdmChatSetting = sGuildLevelSystem->IsAdmChat(player->GetGuild());
        std::string AssistantChatSetting = sGuildLevelSystem->IsAssistantChat(player->GetGuild());

        switch (action)
        {
            /*
                Навигация
            */

        case 100: // Выход
            CloseGossipMenuFor(player);
            break;
        case 101: // Главное меню
            OnGossipHello(player, creature);
            break;

            /*
                Меню
            */
        case 1: // Информация о гильдии
            sGuildLevelSystem->GuildInfo(player);
            SendGossipMenuFor(player, 1, creature->GetGUID());
            break;
        case 2: // Вклад в ги           
            AddGossipItemFor(player, 10, warhead::StringFormat("Вложить всё - |cFF1E90FF%u|r", ItemCount), GOSSIP_SENDER_MAIN, 1000);
            AddGossipItemFor(player, 10, "Вложить выборочно", GOSSIP_SENDER_MAIN, 1001, "Введите количество", 0, true);
            SendGossipMenuFor(player, 1, creature->GetGUID());
            break;
        case 3: // Настройки гильдии
            AddGossipItemFor(player, 10, warhead::StringFormat("%s чат создателя гильдии", AdmChatSetting.c_str()), GOSSIP_SENDER_MAIN, 1001);
            AddGossipItemFor(player, 10, warhead::StringFormat("%s чат заместителя гильдии", AssistantChatSetting.c_str()), GOSSIP_SENDER_MAIN, 1002);
            SendGossipMenuFor(player, 1, creature->GetGUID());
            break;
        case 4: // Информация
            SendGossipMenuFor(player, 60000, creature->GetGUID());
            break;

            /*
                Действия
            */
        case 1000: // Вклад в ги (фул)
            CloseGossipMenuFor(player);
            sGuildLevelSystem->InvestExpFull(player);
            break;
        case 1001: // Смена чата (адм)
            sGuildLevelSystem->ChangeSettingChat(player->GetGuild(), true);
            OnGossipSelect(player, creature, GOSSIP_SENDER_MAIN, 3);
            break;
        case 1002: // Смена чата (зам)
            sGuildLevelSystem->ChangeSettingChat(player->GetGuild(), false);
            OnGossipSelect(player, creature, GOSSIP_SENDER_MAIN, 3);
            break;

        default:
            break;
        }

        return true;
    }

    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, char const* code) override
    {
        ClearGossipMenuFor(player);
        CloseGossipMenuFor(player);

        switch (action)
        {
        case 1001:
            sGuildLevelSystem->InvestExpChoice(player, code);
            break;
        }

        return true;
    }
};

class GuildLevelSystem_World : public WorldScript
{
public:
    GuildLevelSystem_World() : WorldScript("GuildLevelSystem_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddBoolConfig("GLS.Enable");
        sGameConfig->AddIntConfig("GLS.MaxLevel", 30);
        sGameConfig->AddIntConfig("GLS.Exp.ItemID", 37711);
        sGameConfig->AddStringConfig("GLS.ColorChat.Rank0", "|cffff0000");
        sGameConfig->AddStringConfig("GLS.ColorChat.Rank1", "|cffff0000");
    }

    void OnStartup() override
    {
        if (!CONF_GET_BOOL("GLS.Enable"))
            return;

        sGuildLevelSystem->Init();
    }
};

// Group all custom scripts
void AddSC_GuildLevelSystem()
{
    new GuildLevelSystem_World();
    new Kargatum_Guild();
    new Kargatum_Guild_Player();
    new Kargatum_Guild_Creature();
}
