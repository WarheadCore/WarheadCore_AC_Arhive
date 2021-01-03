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
#include "ModuleLocale.h"
#include "Chat.h"
#include "Player.h"
#include "AccountMgr.h"
#include <vector>

enum StringLocales : uint8
{
    BC_LOCALE_COMMAND_DISABLE = 1,
    BC_LOCALE_COMMAND_CANT,

    BC_LOCALE_MAX
};

#define MODULE_NAME "mod-buff-command"

class BuffCommand
{
public:
    static BuffCommand* instance()
    {
        static BuffCommand instance;
        return &instance;
    }

    void LoadDataFromDB()
    {
        uint32 oldMSTime = getMSTime();

        buffs.clear();

        LOG_INFO("module", "Loading buff for command .buff...");

        QueryResult result = WorldDatabase.PQuery("SELECT SpellID FROM `player_buff`");
        if (!result)
        {
            LOG_INFO("module", ">> Loaded 0 buffs. DB table `player_buff` is empty.");
            LOG_INFO("module", "");
            return;
        }

        do
        {
            uint32 spellID = result->Fetch()->GetUInt32();

            auto spell = sSpellStore.LookupEntry(spellID);
            if (!spell)
            {
                LOG_ERROR("module", "-> Spell with number (%u) not found. Skip.", spellID);
                continue;
            }

            buffs.push_back(spellID);

        } while (result->NextRow());

        LOG_INFO("module", ">> Loaded %u buffs in %u ms", static_cast<uint32>(buffs.size()), GetMSTimeDiffToNow(oldMSTime));
        LOG_INFO("module", "");
    }

    void ApplyBuffs(Player* player)
    {
        for (auto const& itr : buffs)
            player->CastSpell(player, itr, true);
    }

private:
    std::vector<uint32> buffs;
};

#define sBC BuffCommand::instance()

class BuffCommand_Command : public CommandScript
{
public:
    BuffCommand_Command() : CommandScript("BuffCommand_Command") {}

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> commandTable = // .commands
        {
            { "buff",    SEC_PLAYER,   false, &HandleBuffCommand, ""}
        };

        return commandTable;
    }

    static bool HandleBuffCommand(ChatHandler* handler, const char* args)
    {
        Player* player = handler->GetSession()->GetPlayer();
        if (!player)
            return false;

        if (!CONF_GET_BOOL("BuffCommand.Enable"))
        {
            sModuleLocale->SendPlayerMessage(player, MODULE_NAME, BC_LOCALE_COMMAND_DISABLE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::string stringArg = (char*)args;

        if (stringArg == "reload" && AccountMgr::IsAdminAccount(player->GetSession()->GetSecurity()))
        {
            LOG_INFO("module", "Re-Loading player buff data...");
            sBC->LoadDataFromDB();
            handler->SendGlobalGMSysMessage("|cff6C8CD5#|cFFFF0000 DB Table|r `player_buff` |cFFFF0000reloaded.|r");
            return true;
        }
        else
        {
            if (player->duel || player->GetMap()->IsBattleArena() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || player->isDead(), player->IsInCombat() || player->IsInFlight())
            {
                sModuleLocale->SendPlayerMessage(player, MODULE_NAME, BC_LOCALE_COMMAND_CANT);
                handler->SetSentErrorMessage(true);
                return false;
            }

            player->RemoveAurasByType(SPELL_AURA_MOUNTED);

            sBC->ApplyBuffs(player);

            return true;
        }
    }
};

class BuffCommand_World : public WorldScript
{
public:
    BuffCommand_World() : WorldScript("BuffCommand_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddOption<bool>("BuffCommand.Enable");
    }

    void OnStartup() override
    {
        if (!CONF_GET_BOOL("BuffCommand.Enable"))
            return;

        sModuleLocale->CheckStrings(MODULE_NAME, BC_LOCALE_MAX);

        sBC->LoadDataFromDB();
    }
};

// Group all custom scripts
void AddSC_BuffCommand()
{
    new BuffCommand_Command();
    new BuffCommand_World();
}
