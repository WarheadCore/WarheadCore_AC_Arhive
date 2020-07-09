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

#include "Config.h"
#include "Errors.h"
#include "Log.h"
#include "Util.h"
#include "StringFormat.h"

ConfigMgr* ConfigMgr::instance()
{
    static ConfigMgr instance;
    return &instance;
}

// Defined here as it must not be exposed to end-users.
bool ConfigMgr::GetValueHelper(const char* name, ACE_TString& result)
{
    GuardType guard(_configLock);

    if (!_config.get())
        return false;

    ACE_TString section_name;
    ACE_Configuration_Section_Key section_key;
    const ACE_Configuration_Section_Key& root_key = _config->root_section();

    int i = 0;

    while (!_config->enumerate_sections(root_key, i, section_name))
    {
        _config->open_section(root_key, section_name.c_str(), 0, section_key);

        if (!_config->get_string_value(section_key, name, result))
            return true;

        ++i;
    }

    return false;
}

bool ConfigMgr::LoadInitial(std::string const& file)
{
    ASSERT(file.c_str());

    GuardType guard(_configLock);

    _config.reset(new ACE_Configuration_Heap());
    if (!_config->open())
        if (LoadData(file))
            return true;

    _config.reset();
    return false;
}

bool ConfigMgr::LoadMore(std::string const& file)
{
    ASSERT(file.c_str());
    ASSERT(_config);

    GuardType guard(_configLock);

    return LoadData(file);
}

bool ConfigMgr::Reload()
{
    if (!LoadAppConfigs())
        return false;

    LoadModulesConfigs();

    return true;
}

bool ConfigMgr::LoadData(std::string const& file)
{
    ACE_Ini_ImpExp config_importer(*_config.get());
    if (!config_importer.import_config(file.c_str()))
        return true;

    return false;
}

std::string ConfigMgr::GetStringDefault(std::string const& name, const std::string& def, bool logUnused /*= true*/)
{
    ACE_TString val;

    if (GetValueHelper(name.c_str(), val))
        return val.c_str();
    else
    {
        if (logUnused)
            LOG_ERROR("server", "-> Not found option '%s'. The default value is used (%s)", name.c_str(), def.c_str());

        return def;
    }
}

bool ConfigMgr::GetBoolDefault(std::string const& name, bool def, bool logUnused /*= true*/)
{
    ACE_TString val;

    if (!GetValueHelper(name.c_str(), val))
    {
        if (logUnused)
            LOG_ERROR("server", "-> Not found option '%s'. The default value is used (%s)", name.c_str(), def ? "Yes" : "No");

        return def;
    }

    return (val == "true" || val == "TRUE" || val == "yes" || val == "YES" || val == "1");
}

int ConfigMgr::GetIntDefault(std::string const& name, int def, bool logUnused /*= true*/)
{
    ACE_TString val;

    if (GetValueHelper(name.c_str(), val))
        return atoi(val.c_str());
    else
    {
        if (logUnused)
            LOG_ERROR("server", "-> Not found option '%s'. The default value is used (%i)", name.c_str(), def);

        return def;
    }
}

float ConfigMgr::GetFloatDefault(std::string const& name, float def, bool logUnused /*= true*/)
{
    ACE_TString val;

    if (GetValueHelper(name.c_str(), val))
        return (float)atof(val.c_str());
    else
    {
        if (logUnused)
            LOG_ERROR("server", "-> Not found option '%s'. The default value is used (%f)", name.c_str(), def);

        return def;
    }
}

std::list<std::string> ConfigMgr::GetKeysByString(std::string const& name)
{
    GuardType guard(_configLock);

    std::list<std::string> keys;
    if (!_config.get())
        return keys;

    ACE_TString section_name;
    ACE_Configuration_Section_Key section_key;
    const ACE_Configuration_Section_Key& root_key = _config->root_section();

    int i = 0;

    while (!_config->enumerate_sections(root_key, i++, section_name))
    {
        _config->open_section(root_key, section_name.c_str(), 0, section_key);

        ACE_TString key_name;
        ACE_Configuration::VALUETYPE type;

        int j = 0;

        while (!_config->enumerate_values(section_key, j++, key_name, type))
        {
            std::string temp = key_name.c_str();

            if (temp.find(name) != std::string::npos)
                keys.push_back(temp);
        }
    }

    return keys;
}

std::string const ConfigMgr::GetConfigPath()
{
#if WH_PLATFORM == WH_PLATFORM_WINDOWS
    return "configs/";
#else
    return std::string(_CONF_DIR) + "/";
#endif
}

void ConfigMgr::SetConfigList(std::string const& fileName, std::string const& modulesConfigList /*= ""*/)
{
    _initConfigFile = fileName;

    if (modulesConfigList.empty())
        return;

    // Clean config list before load
    _modulesConfigFiles.clear();

    Tokenizer configFileList(modulesConfigList, ',');
    for (auto const& itr : configFileList)
        _modulesConfigFiles.push_back(itr);
}

bool ConfigMgr::LoadAppConfigs(std::string const& applicationName /*= "worldserver"*/)
{
    // #1 - Load init config file .conf.dist
    if (!sConfigMgr->LoadInitial(_initConfigFile + ".dist"))
    {
        SYS_LOG_ERROR("");
        SYS_LOG_ERROR("Load config error. Invalid or missing dist configuration file: %s", std::string(_initConfigFile + ".dist").c_str());
        SYS_LOG_ERROR("Verify that the file exists and has \'[%s]' written in the top of the file!", applicationName.c_str());
        return false;
    }

    // #2 - Load .conf file
    if (!sConfigMgr->LoadMore(_initConfigFile))
    {
        SYS_LOG_ERROR("");
        SYS_LOG_ERROR("Load config error. Invalid or missing configuration file: %s", _initConfigFile.c_str());
        SYS_LOG_ERROR("Verify that the file exists and has \'[%s]' written in the top of the file!", applicationName.c_str());
        return false;
    }

    return true;
}

bool ConfigMgr::LoadModulesConfigs()
{
    // If not modules config - load failed
    if (_modulesConfigFiles.empty())
        return false;

    // Start loading module configs
    std::unordered_map<std::string /*module name*/, std::string /*config variant*/> moduleConfigFiles;

    moduleConfigFiles.clear();

    std::string configModulesFolder = "modules/";

    for (auto const& itr : _modulesConfigFiles)
    {
        bool IsExistDefaultConfig = true;
        bool IsExistDistConfig = true;

        std::string moduleName = itr;
        std::string configFile = moduleName + std::string(".conf");
        std::string defaultConfig = GetConfigPath() + configModulesFolder + configFile;
        std::string ConfigFileDist = defaultConfig + std::string(".dist");

        // Load .conf.dist config
        if (!sConfigMgr->LoadMore(ConfigFileDist))
        {
            LOG_ERROR("config", "Invalid or missing dist configuration file: %s", ConfigFileDist.c_str());
            IsExistDistConfig = false;
        }

        // Load .conf config
        if (!sConfigMgr->LoadMore(defaultConfig))
            IsExistDefaultConfig = false;

        // #1 - Not exist .conf and exist .conf.dist
        if (!IsExistDefaultConfig && IsExistDistConfig)
            moduleConfigFiles.insert(std::make_pair(moduleName, ConfigFileDist));
        else if (!IsExistDefaultConfig && !IsExistDistConfig) // #2 - Not exist .conf and not exist .conf.dist
            moduleConfigFiles.insert(std::make_pair(moduleName, "default hardcoded settings"));
        else if (IsExistDefaultConfig && IsExistDistConfig)
            moduleConfigFiles.insert(std::make_pair(moduleName, defaultConfig));
    }

    // If module configs not exist - no load
    if (moduleConfigFiles.empty())
        return false;

    // Print modules configurations
    LOG_INFO("config", "");
    LOG_INFO("config", "Using modules configuration:");

    for (auto const& itr : moduleConfigFiles)
        LOG_INFO("config", "> Module (%s) using (%s)", itr.first.c_str(), itr.second.c_str());

    LOG_INFO("config", "");

    return true;
}
