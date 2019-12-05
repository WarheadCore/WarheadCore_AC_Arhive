/*
 * Copyright (C) 2016+     AzerothCore <www.azerothcore.org>
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 */

#include "Log.h"
#include "DatabaseEnv.h"

void Log::outDB(LogTypes type, const char * str)
{
    if(!str || std::string(str).empty() || type >= MAX_LOG_TYPES)
        return;

    std::string new_str(str);
    LoginDatabase.EscapeString(new_str);

    LoginDatabase.PExecute("INSERT INTO logs (time, realm, type, string) "
        "VALUES (" UI64FMTD ", %u, %u, '%s');", uint64(time(0)), realm, (uint32)type, new_str.c_str());
}
