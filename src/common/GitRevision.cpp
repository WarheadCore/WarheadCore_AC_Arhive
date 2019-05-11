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

#include "GitRevision.h"
#include "revision_data.h"

char const* GitRevision::GetHash()
{
    return _HASH;
}

char const* GitRevision::GetDate()
{
    return _DATE;
}

char const* GitRevision::GetBranch()
{
    return _BRANCH;
}

char const* GitRevision::GetCMakeCommand()
{
    return _CMAKE_COMMAND;
}

char const* GitRevision::GetCMakeVersion()
{
    return _CMAKE_VERSION;
}

char const* GitRevision::GetHostOSVersion()
{
    return _CMAKE_HOST_SYSTEM;
}

char const* GitRevision::GetBuildDirectory()
{
    return _BUILD_DIRECTORY;
}

char const* GitRevision::GetSourceDirectory()
{
    return _SOURCE_DIRECTORY;
}

char const* GitRevision::GetMySQLExecutable()
{
    return _MYSQL_EXECUTABLE;
}

char const* GitRevision::GetFullDatabase()
{
    return _FULL_DATABASE;
}

#if WARHEAD_PLATFORM == WARHEAD_PLATFORM_WINDOWS
#  ifdef _WIN64
#    define WARHEAD_PLATFORM_STR "Win64"
#  else
#    define WARHEAD_PLATFORM_STR "Win32"
#  endif
#elif WARHEAD_PLATFORM == WARHEAD_PLATFORM_APPLE
#  define WARHEAD_PLATFORM_STR "MacOSX"
#elif WARHEAD_PLATFORM == WARHEAD_PLATFORM_INTEL
#  define WARHEAD_PLATFORM_STR "Intel"
#else // WARHEAD_PLATFORM_UNIX
#  define WARHEAD_PLATFORM_STR "Unix"
#endif

#ifndef WARHEAD_API_USE_DYNAMIC_LINKING
#  define WARHEAD_LINKAGE_TYPE_STR "Static"
#else
#  define WARHEAD_LINKAGE_TYPE_STR "Dynamic"
#endif

char const* GitRevision::GetFullVersion()
{
  return "WarheadCore rev. " VER_PRODUCTVERSION_STR
    " (" WARHEAD_PLATFORM_STR ", " _BUILD_DIRECTIVE ", " WARHEAD_LINKAGE_TYPE_STR ")";
}

char const* GitRevision::GetCompanyNameStr()
{
    return VER_COMPANYNAME_STR;
}

char const* GitRevision::GetLegalCopyrightStr()
{
    return VER_LEGALCOPYRIGHT_STR;
}

char const* GitRevision::GetFileVersionStr()
{
    return VER_FILEVERSION_STR;
}

char const* GitRevision::GetProductVersionStr()
{
    return VER_PRODUCTVERSION_STR;
}
