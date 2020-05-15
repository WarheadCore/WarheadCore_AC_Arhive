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
