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

class Kargatum_Guild : public GuildScript
{
public:
    Kargatum_Guild() : GuildScript("Kargatum_Guild") { }

    void OnAddMember(Guild* guild, Player* player, uint8& /*plRank*/) override
    {
        if (!CONF_GET_BOOL("GLS.Enable"))
            return;

        uint32 guildID = guild->GetId();

        sGuildLevelSystem->RescaleCriterias(guildID);
        sGuildLevelSystem->LearnSpellsForPlayer(player, guildID);
    }

    void OnRemoveMember(Guild* guild, Player* player, uint32 lowGuid, bool /*isDisbanding*/, bool /*isKicked*/) override
    {
        if (!CONF_GET_BOOL("GLS.Enable"))
            return;

        uint32 guildID = guild->GetId();

        sGuildLevelSystem->RescaleCriterias(guildID);
        sGuildLevelSystem->UnLearnSpellsForPlayer(lowGuid, guildID);
    }
};

class Kargatum_Guild_Creature : public CreatureScript
{
public:
    Kargatum_Guild_Creature() : CreatureScript("Kargatum_Guild_Creature") { }

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

        AddGossipItemFor(player, 10, "Информация о гильдии", GOSSIP_SENDER_MAIN, 1);
        AddGossipItemFor(player, 10, "Критерии", GOSSIP_SENDER_MAIN, 2);
        AddGossipItemFor(player, 10, "Выход", GOSSIP_SENDER_MAIN, 100);

        SendGossipMenuFor(player, 1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        ClearGossipMenuFor(player);

        if (sender > GLS_GOSSIP_GET_ALL_LINK)
            ABORT();

        // Show details info criteria and rewards
        if (sender == GLS_GOSSIP_SHOW_CRITERIA_SENDER)
        {
            sGuildLevelSystem->ShowCriteriaInfo(player, creature, sender, action);
            return true;
        }
        else if (sender >= GLS_GOSSIP_SHOW_REWARDS_SENDER && sender < GLS_GOSSIP_GET_REWARDS_SENDER + GLS_SPELLS_REWARD_COUNT)
        {
            sGuildLevelSystem->ShowRewardInfo(player, creature, sender, action);
            return true;
        }
        else if (sender >= GLS_GOSSIP_CHOOSE_REWARD_SENDER && sender < GLS_GOSSIP_GET_ALL_REWARDS_SENDER + GLS_SPELLS_REWARD_COUNT/*sender < GLS_GOSSIP_CHOOSE_REWARD_SENDER + GLS_ITEMS_REWARD_CHOOSE_COUNT*/)
        {
            sGuildLevelSystem->GetRewardsCriteria(player, creature, sender, action);
            return true;
        }
        else if (sender == GLS_GOSSIP_GET_ALL_LINK)
        {
            sGuildLevelSystem->GetAllLink(player, creature, sender, action);
            return true;
        }

        // Invest full items
        if (action > GLS_GOSSIP_CRITERIA_ID_FULL)
        {
            sGuildLevelSystem->InvestItemFull(player, creature, sender, action);
            return true;
        }
        else if (action > GLS_GOSSIP_CRITERIA_ID) // Invest choose items
        {
            sGuildLevelSystem->ShowInvestedMenu(player, creature, sender, action);
            return true;
        }

        switch (action)
        {
            case 1:
                AddGossipItemFor(player, 10, Warhead::StringFormat("> Количество участников %u", player->GetGuild()->GetMemberCount()), GOSSIP_SENDER_MAIN, 1);
                AddGossipItemFor(player, 10, ">> В главное меню", GOSSIP_SENDER_MAIN, 99);
                SendGossipMenuFor(player, 1, creature->GetGUID());
                break;
            case 2: // Вклад в ги
                sGuildLevelSystem->ShowAllCriteriaInfo(player, creature);
                break;
            case 3: // Получить награды
                sGuildLevelSystem->ShowAllCriteriaInfo(player, creature);
                break;
            case 4: // Завершить стадию
                sGuildLevelSystem->SetNextStage(player);
                CloseGossipMenuFor(player);
                break;
            case 100: // Выход
                CloseGossipMenuFor(player);
                break;
            case 99: // Главное меню
                OnGossipHello(player, creature);
                break;
            default:
                break;
        }

        return true;
    }

    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, char const* code) override
    {
        ClearGossipMenuFor(player);
        sGuildLevelSystem->InvestItem(player, creature, sender, action, static_cast<uint32>(atoi(code)));

        return true;
    }
};

class GuildLevelSystem_World : public WorldScript
{
public:
    GuildLevelSystem_World() : WorldScript("GuildLevelSystem_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddOption<bool>("GLS.Enable");
        sGameConfig->AddOption<bool>("GLS.Criteria.ShowItems.Enable");
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
    new Kargatum_Guild_Creature();
}
