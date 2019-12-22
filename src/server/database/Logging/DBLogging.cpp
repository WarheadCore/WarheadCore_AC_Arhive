/*
 * Copyright (C) 2016+     AzerothCore <www.azerothcore.org>
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 */

#include "Log.h"
#include "DatabaseEnv.h"

//void Log::_writeDB(std::string const& filter, LogLevel const level, std::string const& message)
//{
//    std::string new_message(message);
//    LoginDatabase.EscapeString(new_message);
//
//    LoginDatabase.PExecute("INSERT INTO logs (Time, Filter, RealmID, LogLevel, Message) VALUES (" UI64FMTD ", '%s', %u, %u, '%s')",
//        uint64(time(nullptr)), filter.c_str(), m_realmID, (uint8)level, new_message.c_str());
//}
