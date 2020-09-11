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

#ifndef _DURATION_H_
#define _DURATION_H_

#include <chrono>

/// Microseconds shorthand typedef.
using Microseconds = std::chrono::microseconds;

/// Milliseconds shorthand typedef.
using Milliseconds = std::chrono::milliseconds;

/// Seconds shorthand typedef.
using Seconds = std::chrono::seconds;

/// Minutes shorthand typedef.
using Minutes = std::chrono::minutes;

/// Hours shorthand typedef.
using Hours = std::chrono::hours;

/// time_point shorthand typedefs
using TimePoint = std::chrono::steady_clock::time_point;
using SystemTimePoint = std::chrono::system_clock::time_point;

/// Makes std::chrono_literals globally available.
using namespace std::chrono_literals;

#endif
