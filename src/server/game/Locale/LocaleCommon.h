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

#ifndef _LOCALE_COMMON_H_
#define _LOCALE_COMMON_H_

#include "Common.h"
#include "WorldPacket.h"
#include <vector>

namespace Warhead::Game::Locale
{
    void AddLocaleString(std::string const& str, LocaleConstant locale, std::vector<std::string>& data);

    class ModulesLocaleTextBuilder
    {
    public:
        typedef std::vector<WorldPacket*> WorldPacketList;
        explicit ModulesLocaleTextBuilder(uint32 textId, std::string const& moduleName, va_list* args = nullptr) : i_textId(textId), i_args(args), _moduleName(moduleName) { }

        void operator()(WorldPacketList& data_list, LocaleConstant loc_idx);
    private:
        char* lineFromMessage(char*& pos) { char* start = strtok(pos, "\n"); pos = nullptr; return start; }

        void do_helper(WorldPacketList& data_list, char* text);

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

        void operator()(Player* player);

    private:
        Builder& i_builder;
        std::vector<WorldPacketList> i_data_cache;
        // 0 = default, i => i-1 locale index
    };
}

#endif // _LOCALE_COMMON_H_
