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

#include "Define.h"
#include <unordered_map>

enum GameIntConfigs
{
    CONFIG_GM_ACCEPT_TICKETS,
    CONFIG_PLAYER_ALLOW_COMMANDS,

    INT_CONFIG_VALUE_COUNT
};

class GameConfig
{
public:
    static GameConfig* instance();

    void Load(bool reload);

    // Add config options
    void AddBoolConfig(std::string const& optionName, bool const& def = false);
    void AddStringConfig(std::string const& optionName, std::string const& def = "");
    void AddIntConfig(std::string const& optionName, int32 const& def = 0);
    void AddFloatConfig(std::string const& optionName, float const& def = 1.0f);

    // Get config options
    bool GetBoolConfig(std::string const& optionName);
    std::string GetStringConfig(std::string const& optionName);
    int32 GetIntConfig(std::string const& optionName);
    float GetFloatConfig(std::string const& optionName);

    void SetBool(std::string const& optionName, bool const& value);
    void SetString(std::string const& optionName, std::string const& value);
    void SetInt(std::string const& optionName, int32 const& value);
    void SetFloat(std::string const& optionName, float const& value);

private:
    void LoadBoolConfigs(bool reload = false);
    void LoadStringConfigs(bool reload = false);
    void LoadIntConfigs(bool reload = false);
    void LoadFloatConfigs(bool reload = false);

    typedef std::unordered_map<std::string, bool> BoolGameConfigs;
    typedef std::unordered_map<std::string, std::string> StringGameConfigs;
    typedef std::unordered_map<std::string, int32> IntGameConfigs;
    typedef std::unordered_map<std::string, float> FloatGameConfigs;

    BoolGameConfigs _boolConfigs;
    StringGameConfigs _stringConfigs;
    IntGameConfigs _intConfigs;
    FloatGameConfigs _floatConfigs;
};

#define sGameConfig GameConfig::instance()

#define CONF_GET_BOOL(__oprionName) sGameConfig->GetBoolConfig(__oprionName)
#define CONF_GET_STR(__oprionName) sGameConfig->GetStringConfig(__oprionName)
#define CONF_GET_INT(__oprionName) sGameConfig->GetIntConfig(__oprionName)
#define CONF_GET_FLOAT(__oprionName) sGameConfig->GetFloatConfig(__oprionName)

#endif // __GAME_CONFIG
