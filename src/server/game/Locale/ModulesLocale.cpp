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

namespace Warhead::Locale
{
    void AddLocaleString(std::string const& str, LocaleConstant locale, std::vector<std::string>& data)
    {
        if (str.empty())
            return;

        if (data.size() <= size_t(locale))
            data.resize(locale + 1);

        data[locale] = str;
    }

    class ModulesLocaleTextBuilder
    {
    public:
        typedef std::vector<WorldPacket*> WorldPacketList;
        explicit ModulesLocaleTextBuilder(uint32 textId, std::string const& moduleName, va_list* args = nullptr) : i_textId(textId), i_args(args), _moduleName(moduleName) { }

        void operator()(WorldPacketList& data_list, LocaleConstant loc_idx)
        {
            char const* text = sModulesLocale->GetModuleString(_moduleName, i_textId, static_cast<uint8>(loc_idx)).value().c_str();

            if (i_args)
            {
                // we need copy va_list before use or original va_list will corrupted
                va_list ap;
                va_copy(ap, *i_args);

                char str[2048];
                vsnprintf(str, 2048, text, ap);
                va_end(ap);

                do_helper(data_list, &str[0]);
            }
            else
                do_helper(data_list, (char*)text);
        }
    private:
        char* lineFromMessage(char*& pos) { char* start = strtok_r(pos, "\n"); pos = nullptr; return start; }

        void do_helper(WorldPacketList& data_list, char* text)
        {
            char* pos = text;

            while (char* line = lineFromMessage(pos))
            {
                WorldPacket* data = new WorldPacket();
                ChatHandler::BuildChatPacket(*data, CHAT_MSG_SYSTEM, LANG_UNIVERSAL, nullptr, nullptr, line);
                data_list.push_back(data);
            }
        }

        uint32 i_textId;
        std::string const& _moduleName;
        va_list* i_args;
    };

    // Prepare using Builder localized packets with caching and send to player
    template<class Builder>
    class LocalizedPacketListDo
    {
    public:
        typedef std::vector<WorldPacket*> WorldPacketList;
        explicit LocalizedPacketListDo(Builder& builder) : i_builder(builder) { }

        ~LocalizedPacketListDo()
        {
            for (size_t i = 0; i < i_data_cache.size(); ++i)
                for (size_t j = 0; j < i_data_cache[i].size(); ++j)
                    delete i_data_cache[i][j];
        }

        void operator()(Player* player)
        {
            LocaleConstant loc_idx = player->GetSession()->GetSessionDbLocaleIndex();
            uint32 cache_idx = loc_idx + 1;
            WorldPacketList* data_list;

            // create if not cached yet
            if (i_data_cache.size() < cache_idx + 1 || i_data_cache[cache_idx].empty())
            {
                if (i_data_cache.size() < cache_idx + 1)
                    i_data_cache.resize(cache_idx + 1);

                data_list = &i_data_cache[cache_idx];
                i_builder(*data_list, loc_idx);
            }
            else
                data_list = &i_data_cache[cache_idx];

            for (size_t i = 0; i < data_list->size(); ++i)
                player->SendDirectMessage((*data_list)[i]);
        }

    private:
        Builder& i_builder;
        std::vector<WorldPacketList> i_data_cache;
        // 0 = default, i => i-1 locale index
    };
}

ModulesLocale* ModulesLocale::instance()
{
    static ModulesLocale instance;
    return &instance;
}

void ModulesLocale::Init()
{
    uint32 oldMSTime = getMSTime();

    LOG_INFO("server.loading", ">> Loading modules strings");
    LoadModuleString();
}

void ModulesLocale::AddModuleString(std::string const& moduleName, ModuleStringContainer& data)
{
    if (data.empty())
    {
        LOG_ERROR("locales.modules", "ModuleStringContainer& data for module (%s) is empty!", moduleName.c_str());
        return;
    }

    _modulesStringStore.insert(std::make_pair(moduleName, data));

    LOG_DEBUG("locales.modules", "> ModulesLocales: added %u strings for (%s) module", static_cast<uint32>(data.size()), moduleName.c_str());
}

std::optional<std::string> ModulesLocale::GetModuleString(std::string const& moduleName, uint32 id, uint8 _locale) const
{
    if (moduleName.empty())
    {
        LOG_ERROR("locales.modules", "> ModulesLocales: moduleName is empty!");
        return nullopt;
    }

    auto const& itr = _modulesStringStore.find(moduleName);
    if (itr == _modulesStringStore.end())
    {
        LOG_FATAL("locales.modules", "> ModulesLocales: Not found strings for module (%s)", moduleName.c_str());
        return nullopt;
    }

    auto const& itr2 = itr->second.find(id);
    if (itr2 == itr->second.end())
    {
        LOG_FATAL("locales.modules", "> ModulesLocales: Not found string (%u) for module (%s)", id, moduleName.c_str());
        return nullopt;
    }

    return itr2->second.GetText(_locale);
}

uint32 ModulesLocale::GetStringsCount(std::string const& moduleName)
{
    if (moduleName.empty())
    {
        LOG_ERROR("locales.modules", "> ModulesLocales: _moduleName is empty!");
        return 0;
    }

    auto const& itr = _modulesStringStore.find(moduleName);
    if (itr == _modulesStringStore.end())
    {
        LOG_FATAL("locales.modules", "> ModulesLocales: Not found strings for module (%s)", moduleName.c_str());
        return 0;
    }

    return static_cast<uint32>(itr->second.size());
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
        std::string const& moduleName = itr;

        result = WorldDatabase.PQuery("SELECT `ID`, `Locale`, `Text` FROM `string_module` WHERE `ModuleName` = '%s'", moduleName.c_str());
        if (!result)
        {
            LOG_WARN("sql.sql", "> Strings for module %s is bad!", moduleName.c_str());
            return;
        }

        do
        {
            Field* fields = result->Fetch();

            Warhead::Locale::AddLocaleString(fields[2].GetString(), GetLocaleByName(fields[1].GetString()), _tempStore[fields[0].GetUInt32()].Content);
            countAll++;

        } while (result->NextRow());

        AddModuleString(moduleName, _tempStore);
    }

    LOG_INFO("server.loading", ">> Loaded %u module strings for %u modules in %u ms", countAll, static_cast<uint32>(_modulesStringStore.size()), GetMSTimeDiffToNow(oldMSTime));
    LOG_INFO("server.loading", "");
}

void ModulesLocale::SendPlayerMessage(Player* player, std::string const& moduleName, uint32 id, ...)
{
    if (moduleName.empty())
    {
        LOG_ERROR("locales.modules", "> ModulesLocales: _moduleName is empty!");
        return;
    }

    va_list ap;
    va_start(ap, id);

    Warhead::Locale::ModulesLocaleTextBuilder wt_builder(id, moduleName, &ap);
    Warhead::Locale::LocalizedPacketListDo<Warhead::Locale::ModulesLocaleTextBuilder> wt_do(wt_builder);

    wt_do(player);

    va_end(ap);
}

void ModulesLocale::SendGlobalMessage(bool gmOnly, std::string const& moduleName, uint32 id, ...)
{
    if (moduleName.empty())
    {
        LOG_ERROR("locales.modules", "> ModulesLocales: _moduleName is empty!");
        return;
    }

    va_list ap;
    va_start(ap, id);

    Warhead::Locale::ModulesLocaleTextBuilder wt_builder(id, moduleName, &ap);
    Warhead::Locale::LocalizedPacketListDo<Warhead::Locale::ModulesLocaleTextBuilder> wt_do(wt_builder);

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
