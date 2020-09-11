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

#ifndef CONFIG_H
#define CONFIG_H

#include "Common.h"
#include <list>

class WH_COMMON_API ConfigMgr
{
    ConfigMgr() = default;
    ConfigMgr(ConfigMgr const&) = delete;
    ConfigMgr& operator=(ConfigMgr const&) = delete;
    ~ConfigMgr() = default;

public:
    static ConfigMgr* instance();

    /// Method used only for loading main configuration files (authserver.conf and worldserver.conf)
    bool LoadInitial(std::string const& file);

    /**
     * This method loads additional configuration files
     * It is recommended to use this method in WorldScript::OnConfigLoad hooks
     *
     * @return true if loading was successful
     */
    bool LoadMore(std::string const& file);

    bool Reload();

    bool LoadAppConfigs(std::string const& applicationName = "worldserver");
    bool LoadModulesConfigs();

    std::string GetStringDefault(std::string const& name, const std::string& def, bool logUnused = true);
    bool GetBoolDefault(std::string const& name, bool def, bool logUnused = true);
    int GetIntDefault(std::string const& name, int def, bool logUnused = true);
    float GetFloatDefault(std::string const& name, float def, bool logUnused = true);

    std::list<std::string> GetKeysByString(std::string const& name);

    bool isDryRun() { return dryRun; }
    void setDryRun(bool mode) { dryRun = mode; }

    void SetConfigList(std::string const& fileName, std::string const& modulesConfigList = "");

    std::string const GetConfigPath();

private:
    bool dryRun = false;

    bool LoadData(std::string const& file);
};

#define sConfigMgr ConfigMgr::instance()

#endif
