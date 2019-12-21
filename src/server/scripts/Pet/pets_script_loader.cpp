/*
 * Copyright (C) 2016+     AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-GPL2
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 */

// This is where scripts' loading functions should be declared:
// Pets
void AddSC_deathknight_pet_scripts();
void AddSC_generic_pet_scripts();
void AddSC_hunter_pet_scripts();
void AddSC_mage_pet_scripts();
void AddSC_priest_pet_scripts();
void AddSC_shaman_pet_scripts();

// The name of this function should match:
// void Add${NameOfDirectory}Scripts()
void AddPetScripts()
{
    AddSC_deathknight_pet_scripts();
    AddSC_generic_pet_scripts();
    AddSC_hunter_pet_scripts();
    AddSC_mage_pet_scripts();
    AddSC_priest_pet_scripts();
    AddSC_shaman_pet_scripts();
}
