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

#include "LocaleCommon.h"
#include "Chat.h"
#include "ModuleLocale.h"

void Warhead::Game::Locale::AddLocaleString(std::string const& str, LocaleConstant locale, std::vector<std::string>& data)
{
    if (str.empty())
        return;

    if (data.size() <= size_t(locale))
        data.resize(locale + 1);

    data[locale] = str;
}

void Warhead::Game::Locale::ModulesLocaleTextBuilder::operator()(WorldPacketList& data_list, LocaleConstant loc_idx)
{
    char const* text = sModuleLocale->GetModuleString(_moduleName, i_textId, static_cast<uint8>(loc_idx)).value().c_str();

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

void Warhead::Game::Locale::ModulesLocaleTextBuilder::do_helper(WorldPacketList& data_list, char* text)
{
    char* pos = text;

    while (char* line = lineFromMessage(pos))
    {
        WorldPacket* data = new WorldPacket();
        ChatHandler::BuildChatPacket(*data, CHAT_MSG_SYSTEM, LANG_UNIVERSAL, nullptr, nullptr, line);
        data_list.push_back(data);
    }
}
