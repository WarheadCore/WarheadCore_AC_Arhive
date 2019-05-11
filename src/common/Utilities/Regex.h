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

#ifndef _WC_REGEX_H_
#define _WC_REGEX_H_

#ifndef WC_HAS_BROKEN_WSTRING_REGEX
#include <regex>
#define WC_REGEX_NAMESPACE std
#else
#include <boost/regex.hpp>
#define WC_REGEX_NAMESPACE boost
#endif

// regex compatibility layer, required for clang building with libstdc++-4.9
namespace Warhead
{
    using regex = WC_REGEX_NAMESPACE :: regex;
    using wregex = WC_REGEX_NAMESPACE :: wregex;

    using :: WC_REGEX_NAMESPACE :: regex_match;
    using :: WC_REGEX_NAMESPACE :: regex_search;
}

#endif // _WC_REGEX_H_
