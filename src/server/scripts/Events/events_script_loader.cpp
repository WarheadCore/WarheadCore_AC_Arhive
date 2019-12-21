/*
 * Copyright (C) 2016+     AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-GPL2
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 */

// This is where scripts' loading functions should be declared:
void AddSC_event_brewfest_scripts();
void AddSC_event_hallows_end_scripts();
void AddSC_event_pilgrims_end_scripts();
void AddSC_event_winter_veil_scripts();
void AddSC_event_love_in_the_air();
void AddSC_event_midsummer_scripts();
void AddSC_event_childrens_week();

// The name of this function should match:
// void Add${NameOfDirectory}Scripts()
void AddEventsScripts()
{
    AddSC_event_brewfest_scripts();
    AddSC_event_hallows_end_scripts();
    AddSC_event_pilgrims_end_scripts();
    AddSC_event_winter_veil_scripts();
    AddSC_event_love_in_the_air();
    AddSC_event_midsummer_scripts();
    AddSC_event_childrens_week();
}
