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
        uint32 maxLevel = CONF_GET_INT("MaxPlayerLevel");

        if (playerLevel >= maxLevel)
            sModuleLocale->SendPlayerMessage(player, MODULE_NAME, ILU_LOCALE_MAX_LEVEL, playerLevel);
        else if (playerLevel < maxLevel)
        {
            uint8 configLevel = static_cast<uint8>(CONF_GET_INT("ILU.LevelUP"));
            uint8 newLevel = !configLevel ? CONF_GET_INT("MaxPlayerLevel") : configLevel;

            player->GiveLevel(newLevel);
            player->SetUInt32Value(PLAYER_XP, 0);

            sModuleLocale->SendPlayerMessage(player, MODULE_NAME, ILU_LOCALE_GET_LEVEL, newLevel);
        }

        if (CONF_GET_BOOL("ILU.SetMaxSkills.Enable"))
            SetMaxWeaponSkills(player);

        if (CONF_GET_BOOL("ILU.Teleport.Enable"))
            PlayerTeleport(player);

        player->DestroyItemCount(item->GetEntry(), 1, true);

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

        uint32 options = 0;

        if (player->getClass() == CLASS_DEATH_KNIGHT)
            options = TELE_TO_SKIP_START_ZONE_DK;

        player->TeleportTo(WorldLocation(mapID, posX, posY, posZ, orientation), options);
    }

    void SetMaxWeaponSkills(Player* player)
    {
        auto LearnWeaponSkill = [&](std::initializer_list<uint32> spellList)
        {
            for (auto const& spellID : spellList)
            {
                if (player->HasSpell(spellID))
                    continue;

                player->learnSpell(spellID);
            }
        };

        switch (player->getClass())
        {
        case CLASS_WARRIOR:
            LearnWeaponSkill({ 1180, 196, 201, 198, 197, 202, 199, 200, 227, 2567, 5011, 264, 266, 15590 });
            break;
        case CLASS_PALADIN:
            LearnWeaponSkill({ 196, 201, 198, 197, 202, 199, 200 });
            break;
        case CLASS_HUNTER:
            LearnWeaponSkill({ 1180, 196, 201, 197, 202, 200, 227, 2567, 5011, 264, 266, 15590 });
            break;
        case CLASS_ROGUE:
            LearnWeaponSkill({ 196, 201, 198, 2567, 5011, 264, 266, 15590 });
            break;
        case CLASS_PRIEST:
            LearnWeaponSkill({ 1180, 198, 227, 5009 });
            break;
        case CLASS_DEATH_KNIGHT:
            LearnWeaponSkill({ 196, 201, 198, 197, 202, 199, 200 });
            break;
        case CLASS_SHAMAN:
            LearnWeaponSkill({ 1180, 196, 198, 197, 199, 227, 15590 });
            break;
        case CLASS_MAGE:
            LearnWeaponSkill({ 1180, 201, 227, 5009 });
            break;
        case CLASS_WARLOCK:
            LearnWeaponSkill({ 1180, 201, 227, 5009 });
            break;
        case CLASS_DRUID:
            LearnWeaponSkill({ 198, 199, 200, 227, 15590 });
            break;
        default:
            break;
        }

        player->SaveToDB(false, false);
        player->UpdateSkillsToMaxSkillsForLevel();
    }
};

class ItemLevelUp_World : public WorldScript
{
public:
    ItemLevelUp_World() : WorldScript("ItemLevelUp_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddOption<bool>("ILU.Enable");
        sGameConfig->AddOption<bool>("ILU.Teleport.Enable");
        sGameConfig->AddOption<bool>("ILU.SetMaxSkills.Enable");
        sGameConfig->AddOption<int32>("ILU.LevelUP");
        sGameConfig->AddOption<std::string>("ILU.Teleport.Location");
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
