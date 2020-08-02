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

#ifndef _DATABASEENV_H_
#define _DATABASEENV_H_

#include "Common.h"
#include "Errors.h"
#include "Log.h"

#include "Field.h"
#include "QueryResult.h"

#include "MySQLThreading.h"
#include "Transaction.h"

#define _LIKE_           "LIKE"
#define _TABLE_SIM_      "`"
#define _CONCAT3_(A, B, C) "CONCAT( " A ", " B ", " C " )"
#define _OFFSET_         "LIMIT %d, 1"

#include "LoginDatabase.h"
#include "CharacterDatabase.h"
#include "WorldDatabase.h"

/// Accessor to the world database
WH_DATABASE_API extern WorldDatabaseWorkerPool WorldDatabase;

/// Accessor to the character database
WH_DATABASE_API extern CharacterDatabaseWorkerPool CharacterDatabase;

/// Accessor to the realm/login database
WH_DATABASE_API extern LoginDatabaseWorkerPool LoginDatabase;

#endif
