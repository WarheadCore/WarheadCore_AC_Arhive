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

#ifndef _MODULES_LOCALE_H_
#define _MODULES_LOCALE_H_

#include "Common.h"
#include "Player.h"
#include <vector>

struct ModuleString
{
    ModuleString()
    {
        Content.resize(DEFAULT_LOCALE + 1);
    }

    std::vector<std::string> Content;

    std::string GetText(uint8 locale = 0) const
    {
        if (Content.size() > size_t(locale) && !Content[locale].empty())
            return Content[locale];

        if (!Content[0].empty())
            return Content[0];

        return "<error>";
    }
};

class ModulesLocale
{
private:
    ModulesLocale();
    ~ModulesLocale() = default;

public:
    static ModulesLocale* instance();

    void Init();

    void AddLocaleString(std::string const& str, LocaleConstant locale, std::vector<std::string>& data);

    inline void GetLocaleString(const std::vector<std::string>& data, int loc_idx, std::string& value)
    {
        if (data.size() > size_t(loc_idx) && !data[loc_idx].empty())
            value = data[loc_idx];
    }

    LocaleConstant GetDBCLocaleIndex() const { return DBCLocaleIndex; }
    void SetDBCLocaleIndex(LocaleConstant locale) { DBCLocaleIndex = locale; }

    // Module strings
    void LoadModuleString();
    std::string GetModuleString(std::string _moduleName, uint32 id, uint8 _locale) const;

    // Chat func
    template<typename... Args>
    void SendPlayerMessage(Player* player, std::string moduleName, uint32 id, Args&& ... args)
    {
        SendPlayerMessage(player, Warhead::StringFormat(GetModuleString(moduleName, id, uint8(player->GetSession()->GetSessionDbLocaleIndex())), std::forward<Args>(args)...));
    }

    template<typename... Args>
    void SendGlobalMessage(std::string moduleName, uint32 id, bool gmOnly, Args&& ... args)
    {
        SendGlobalMessage(gmOnly, Warhead::StringFormat(GetModuleString(moduleName, id, DBCLocaleIndex), std::forward<Args>(args)...));
    }

private:

    LocaleConstant DBCLocaleIndex;

    // Module string
    using ModuleStringContainer = std::unordered_map<uint32, ModuleString>;
    using AllModulesStringContainer = std::unordered_map<std::string, ModuleStringContainer>;

    AllModulesStringContainer _modulesStringStore;

    void AddModuleString(std::string const& moduleName, ModuleStringContainer& data);
    void SendPlayerMessage(Player* player, std::string&& message);
    void SendGlobalMessage(bool gmOnly, std::string&& message);
};

#define sModulesLocale GameLocale::instance()

#endif // _MODULES_LOCALE_H_
