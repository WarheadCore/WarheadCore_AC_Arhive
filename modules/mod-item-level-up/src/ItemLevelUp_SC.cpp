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

#include "ScriptMgr.h"
#include "GameConfig.h"
#include "Player.h"
#include "ModuleLocale.h"
#include "Tokenize.h"
#include "StringConvert.h"

enum StringLocales : uint8
{
    ILU_LOCALE_MAX_LEVEL = 1,
    ILU_LOCALE_GET_LEVEL,

    ILU_LOCALE_MAX
};

#define MODULE_NAME "mod-item-level-up"

class ItemLevelUp_Item : public ItemScript
{
public:
    ItemLevelUp_Item() : ItemScript("ItemLevelUp_Item") {}

    bool OnUse(Player* player, Item* item, const SpellCastTargets& /*Targets*/) override
    {
        if (!CONF_GET_BOOL("ILU.Enable"))
            return false;

        uint8 playerLevel = player->getLevel();
        if (playerLevel >= CONF_GET_INT("MaxPlayerLevel"))
        {
            sModuleLocale->SendPlayerMessage(player, MODULE_NAME, ILU_LOCALE_MAX_LEVEL, playerLevel);
            return false;
        }

        uint8 configLevel = static_cast<uint8>(CONF_GET_INT("ILU.LevelUP"));
        uint8 newLevel = !configLevel ? CONF_GET_INT("MaxPlayerLevel") : configLevel;

        player->GiveLevel(newLevel);
        player->SetUInt32Value(PLAYER_XP, 0);
        player->DestroyItemCount(item->GetEntry(), 1, true);

        sModuleLocale->SendPlayerMessage(player, MODULE_NAME, ILU_LOCALE_GET_LEVEL, newLevel);

        if (CONF_GET_BOOL("ILU.Teleport.Enable"))
            PlayerTeleport(player);

        return true;
    }

private:
    void PlayerTeleport(Player* player)
    {
        if (!player)
            return;

        std::string const& locationInfo = CONF_GET_STR("ILU.Teleport.Location");

        std::vector<std::string_view> tokens = Warhead::Tokenize(locationInfo, ' ', false);

        // If invalid - skip
        if (tokens.size() != 5)
            return;

        uint32 mapID = *Warhead::StringTo<uint32>(tokens[0]);
        float posX = *Warhead::StringTo<float>(tokens[1]);
        float posY = *Warhead::StringTo<float>(tokens[2]);
        float posZ = *Warhead::StringTo<float>(tokens[3]);
        float orientation = *Warhead::StringTo<float>(tokens[4]);

        LOG_INFO("server", "%u, %f, %f, %f, %f", mapID, posX, posY, posZ, orientation);

        player->TeleportTo(WorldLocation(mapID, posX, posY, posZ, orientation));
    }
};

class ItemLevelUp_World : public WorldScript
{
public:
    ItemLevelUp_World() : WorldScript("ItemLevelUp_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddBoolConfig("ILU.Enable");
        sGameConfig->AddBoolConfig("ILU.Teleport.Enable");
        sGameConfig->AddIntConfig("ILU.LevelUP");
        sGameConfig->AddStringConfig("ILU.Teleport.Location");
    }

    void OnStartup() override
    {
        sModuleLocale->CheckStrings(MODULE_NAME, ILU_LOCALE_MAX);
    }
};

// Group all custom scripts
void AddSC_ItemLevelUp()
{
    new ItemLevelUp_Item();
    new ItemLevelUp_World();
}
