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
#include <unordered_map>
#include <optional>

struct ModuleString;

class ModuleLocale
{
private:
    ModuleLocale() = default;
    ~ModuleLocale() = default;

public:
    static ModuleLocale* instance();

    void Init();
    void LoadModuleString();
    void CheckStrings(std::string const& moduleName, uint8 maxString);

    std::optional<std::string> GetModuleString(std::string const& moduleName, uint32 id, uint8 _locale) const;
    uint32 GetStringsCount(std::string const& moduleName);

    // Chat func
    void SendPlayerMessage(Player* player, std::string const& moduleName, uint32 id, ...);
    void SendGlobalMessage(bool gmOnly, std::string const& moduleName, uint32 id, ...);

private:
    using ModuleStringContainer = std::unordered_map<uint32, ModuleString>;
    using AllModulesStringContainer = std::unordered_map<std::string, ModuleStringContainer>;

    AllModulesStringContainer _modulesStringStore;

    void AddModuleString(std::string const& moduleName, ModuleStringContainer& data);
};

#define sModuleLocale ModuleLocale::instance()

#endif // _MODULES_LOCALE_H_
