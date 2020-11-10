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

#include "ModuleLocale.h"
#include "GameLocale.h"
#include "LocaleCommon.h"
#include "AccountMgr.h"
#include "Chat.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "Player.h"
#include "World.h"

ModuleLocale* ModuleLocale::instance()
{
    static ModuleLocale instance;
    return &instance;
}

void ModuleLocale::Init()
{
    uint32 oldMSTime = getMSTime();

    LOG_INFO("server.loading", ">> Loading modules strings");
    LoadModuleString();
}

void ModuleLocale::CheckStrings(std::string const& moduleName, uint32 maxString)
{
    uint32 stringCount = GetStringsCount(moduleName);
    maxString--;

    if (stringCount != maxString)
        LOG_FATAL("locale.module", "> Strings locale (%u) != (%u) for module (%s)", stringCount, maxString, moduleName.c_str());
}

void ModuleLocale::AddModuleString(std::string const& moduleName, ModuleStringContainer& data)
{
    if (data.empty())
    {
        LOG_ERROR("locale.module", "ModuleStringContainer& data for module (%s) is empty!", moduleName.c_str());
        return;
    }

    _modulesStringStore.insert(std::make_pair(moduleName, data));

    LOG_DEBUG("locale.module", "> ModulesLocales: added %u strings for (%s) module", static_cast<uint32>(data.size()), moduleName.c_str());
}

std::optional<std::string> ModuleLocale::GetModuleString(std::string const& moduleName, uint32 id, uint8 _locale) const
{
    if (moduleName.empty())
    {
        LOG_ERROR("locale.module", "> ModulesLocales: moduleName is empty!");
        return nullopt;
    }

    auto const& itr = _modulesStringStore.find(moduleName);
    if (itr == _modulesStringStore.end())
    {
        LOG_FATAL("locale.module", "> ModulesLocales: Not found strings for module (%s)", moduleName.c_str());
        return nullopt;
    }

    auto const& itr2 = itr->second.find(id);
    if (itr2 == itr->second.end())
    {
        LOG_FATAL("locale.module", "> ModulesLocales: Not found string (%u) for module (%s)", id, moduleName.c_str());
        return nullopt;
    }

    return itr2->second.GetText(_locale);
}

uint32 ModuleLocale::GetStringsCount(std::string const& moduleName)
{
    if (moduleName.empty())
    {
        LOG_ERROR("locale.module", "> ModulesLocales: _moduleName is empty!");
        return 0;
    }

    auto const& itr = _modulesStringStore.find(moduleName);
    if (itr == _modulesStringStore.end())
    {
        LOG_FATAL("locale.module", "> ModulesLocales: Not found strings for module (%s)", moduleName.c_str());
        return 0;
    }

    return static_cast<uint32>(itr->second.size());
}

void ModuleLocale::LoadModuleString()
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
        std::string const& moduleName = itr;

        result = WorldDatabase.PQuery("SELECT `ID`, `Locale`, `Text` FROM `string_module` WHERE `ModuleName` = '%s'", moduleName.c_str());
        if (!result)
        {
            LOG_WARN("sql.sql", "> Strings for module %s is bad!", moduleName.c_str());
            return;
        }

        _tempStore.clear();

        do
        {
            Field* fields = result->Fetch();

            Warhead::Game::Locale::AddLocaleString(fields[2].GetString(), GetLocaleByName(fields[1].GetString()), _tempStore[fields[0].GetUInt32()].Content);
            countAll++;

        } while (result->NextRow());

        AddModuleString(moduleName, _tempStore);
    }

    LOG_INFO("server.loading", ">> Loaded %u module strings for %u modules in %u ms", countAll, static_cast<uint32>(_modulesStringStore.size()), GetMSTimeDiffToNow(oldMSTime));
    LOG_INFO("server.loading", "");
}

void ModuleLocale::SendPlayerMessage(Player* player, std::string const& moduleName, uint32 id, ...)
{
    if (moduleName.empty())
    {
        LOG_ERROR("locale.module", "> ModulesLocales: _moduleName is empty!");
        return;
    }

    va_list ap;
    va_start(ap, id);

    Warhead::Game::Locale::ModulesLocaleTextBuilder wt_builder(id, moduleName, &ap);
    Warhead::Game::Locale::LocalizedPacketListDo<Warhead::Game::Locale::ModulesLocaleTextBuilder> wt_do(wt_builder);

    wt_do(player);

    va_end(ap);
}

void ModuleLocale::SendGlobalMessage(bool gmOnly, std::string const& moduleName, uint32 id, ...)
{
    if (moduleName.empty())
    {
        LOG_ERROR("locale.module", "> ModulesLocales: _moduleName is empty!");
        return;
    }

    va_list ap;
    va_start(ap, id);

    Warhead::Game::Locale::ModulesLocaleTextBuilder wt_builder(id, moduleName, &ap);
    Warhead::Game::Locale::LocalizedPacketListDo<Warhead::Game::Locale::ModulesLocaleTextBuilder> wt_do(wt_builder);

    for (auto const& itr : sWorld->GetAllSessions())
    {
        auto session = itr.second;
        auto player = itr.second->GetPlayer();

        if (!session || !player || !player->IsInWorld())
            continue;

        if (AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity()) && gmOnly)
            continue;

        wt_do(player);
    }

    va_end(ap);
}
