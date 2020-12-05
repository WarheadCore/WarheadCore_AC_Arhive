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

#ifndef __GAME_CONFIG
#define __GAME_CONFIG

#include "Common.h"
#include <optional>

class GameConfig
{
public:
    static GameConfig* instance();

    void Load(bool reload);

    // Add config option
    template<typename T>
    void AddOption(std::string const& optionName, std::optional<T> const& def = std::nullopt) const;

    // Get config options
    template<typename T>
    T GetOption(std::string const& optionName, std::optional<T> const& def = std::nullopt) const;

    // Set config option
    template<typename T>
    void SetOption(std::string const& optionName, T const& value) const;

private:
    void LoadBoolConfigs(bool reload = false);
    void LoadStringConfigs(bool reload = false);
    void LoadIntConfigs(bool reload = false);
    void LoadFloatConfigs(bool reload = false);

    void CheckOptions(bool reload = false);
};

#define sGameConfig GameConfig::instance()

#define CONF_GET_BOOL(__optionName) sGameConfig->GetOption<bool>(__optionName)
#define CONF_GET_STR(__optionName) sGameConfig->GetOption<std::string>(__optionName)
#define CONF_GET_INT(__optionName) sGameConfig->GetOption<int32>(__optionName)
#define CONF_GET_UINT(__optionName) sGameConfig->GetOption<uint32>(__optionName)
#define CONF_GET_FLOAT(__optionName) sGameConfig->GetOption<float>(__optionName)
#define CONF_GET_DOUBLE(__optionName) sGameConfig->GetOption<double>(__optionName)

#endif // __GAME_CONFIG
