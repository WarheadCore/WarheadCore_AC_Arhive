/*
* Copyright (C) 2016+     WarheadCore <www.Warheadcore.org>
* Copyright (C) 2008-2018 TrinityCore <http://www.trinitycore.org/>
*/

#ifndef __GITREVISION_H__
#define __GITREVISION_H__

#include "Define.h"

namespace GitRevision
{
    WH_COMMON_API char const* GetHash();
    WH_COMMON_API char const* GetDate();
    WH_COMMON_API char const* GetBranch();
    WH_COMMON_API char const* GetCMakeCommand();
    WH_COMMON_API char const* GetCMakeVersion();
    WH_COMMON_API char const* GetHostOSVersion();
    WH_COMMON_API char const* GetBuildDirectory();
    WH_COMMON_API char const* GetSourceDirectory();
    WH_COMMON_API char const* GetMySQLExecutable();
    WH_COMMON_API char const* GetFullVersion();
    WH_COMMON_API char const* GetCompanyNameStr();
    WH_COMMON_API char const* GetLegalCopyrightStr();
    WH_COMMON_API char const* GetFileVersionStr();
    WH_COMMON_API char const* GetProductVersionStr();
}

#endif
