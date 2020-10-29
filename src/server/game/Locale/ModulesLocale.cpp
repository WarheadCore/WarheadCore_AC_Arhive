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

#include "ModulesLocale.h"
#include "GameLocale.h"
#include "AccountMgr.h"
#include "Chat.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "Player.h"
#include "World.h"

ModulesLocale::ModulesLocale()
{
    DBCLocaleIndex = LOCALE_enUS;
}

ModulesLocale* ModulesLocale::instance()
{
    static ModulesLocale instance;
    return &instance;
}

void ModulesLocale::Init()
{
    uint32 oldMSTime = getMSTime();

    // Get once for all the locale index of DBC language (console/broadcasts)
    DBCLocaleIndex = sWorld->GetDefaultDbcLocale();
    LOG_INFO("server.loading", ">> Localization strings loaded in %u ms", GetMSTimeDiffToNow(oldMSTime));
    LOG_INFO("server.loading", "");
}

// Module string
void ModulesLocale::AddModuleString(std::string const& moduleName, ModuleStringContainer& data)
{
    if (data.empty())
    {
        LOG_ERROR("game.locale", "ModuleStringContainer& data for module (%s) is empty!", moduleName.c_str());
        return;
    }

    _modulesStringStore.insert(std::make_pair(moduleName, data));
}

std::string ModulesLocale::GetModuleString(std::string _moduleName, uint32 id, uint8 _locale) const
{
    auto const& itr = _modulesStringStore.find(_moduleName);
    if (itr == _modulesStringStore.end())
        return "<error>";

    auto const& itr2 = itr->second.find(id);
    if (itr2 == itr->second.end())
        return "<error>";

    return itr2->second.GetText(_locale);
}

void ModulesLocale::LoadModuleString()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT DISTINCT `ModuleName` FROM `string_module`");
    if (!result)
    {
        LOG_WARN("sql.sql", "> DB table `string_module` is empty");
        return;
    }

    ModuleStringContainer _tempStore;
    std::vector<std::string> _localesModuleList;
    uint32 countAll = 0;

    // Add module list
    do
    {
        _localesModuleList.push_back(result->Fetch()->GetString());

    } while (result->NextRow());

    for (auto const& itr : _localesModuleList)
    {
        std::string moduleName = itr;

        result = WorldDatabase.PQuery("SELECT `ID`, `Locale`, `Text` FROM `string_module` WHERE `ModuleName` = '%s'", moduleName.c_str());
        if (!result)
        {
            LOG_WARN("sql.sql", "> Strings for module %s is bad!", moduleName.c_str());
            return;
        }

        do
        {
            Field* fields = result->Fetch();

            AddLocaleString(fields[2].GetString(), GetLocaleByName(fields[1].GetString()), _tempStore[fields[0].GetUInt32()].Content);
            countAll++;

        } while (result->NextRow());

        AddModuleString(moduleName, _tempStore);
    }

    LOG_INFO("server.loading", ">> Loaded %u module strings for %u modules in %u ms", countAll, static_cast<uint32>(_modulesStringStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void ModulesLocale::SendPlayerMessage(Player* player, std::string&& message)
{
    ChatHandler(player->GetSession()).SendSysMessage(message.c_str());
}

void ModulesLocale::SendGlobalMessage(bool gmOnly, std::string&& message)
{
    WorldPacket data;

    for (auto const& itr : sWorld->GetAllSessions())
    {
        auto session = itr.second;
        auto player = itr.second->GetPlayer();

        if (!session || !player || !player->IsInWorld())
            continue;

        if (gmOnly && AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity()))
            continue;

        ChatHandler::BuildChatPacket(data, CHAT_MSG_SYSTEM, LANG_UNIVERSAL, nullptr, nullptr, message);
        sWorld->SendGlobalMessage(&data);
    }
}
