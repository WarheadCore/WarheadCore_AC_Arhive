/*
 * Copyright (C) 2008-2019 TrinityCore <https://www.trinitycore.org/>
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

#ifndef __GITREVISION_H__
#define __GITREVISION_H__

#include "Define.h"

namespace GitRevision
{
    WC_COMMON_API char const* GetHash();
    WC_COMMON_API char const* GetDate();
    WC_COMMON_API char const* GetBranch();
    WC_COMMON_API char const* GetCMakeCommand();
    WC_COMMON_API char const* GetCMakeVersion();
    WC_COMMON_API char const* GetHostOSVersion();
    WC_COMMON_API char const* GetBuildDirectory();
    WC_COMMON_API char const* GetSourceDirectory();
    WC_COMMON_API char const* GetMySQLExecutable();
    WC_COMMON_API char const* GetFullDatabase();
    WC_COMMON_API char const* GetFullVersion();
    WC_COMMON_API char const* GetCompanyNameStr();
    WC_COMMON_API char const* GetLegalCopyrightStr();
    WC_COMMON_API char const* GetFileVersionStr();
    WC_COMMON_API char const* GetProductVersionStr();
}

#endif
