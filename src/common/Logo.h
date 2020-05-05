/*
 *  Copyright (C) 2019+ WarheadCore
 */

#ifndef _LOGO_H_
#define _LOGO_H_

#include "Define.h"

namespace acore
{
    namespace Logo
    {
        AC_COMMON_API void Show(char const* applicationName, char const* configName, void(*log)(char const* text));
    }
}

#endif
