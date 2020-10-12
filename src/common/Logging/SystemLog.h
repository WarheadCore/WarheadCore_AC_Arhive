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

#ifndef _SYSTEM_LOG_H_
#define _SYSTEM_LOG_H_

#include "Common.h"
#include "StringFormat.h"

class WH_COMMON_API SystemLog
{
private:
    SystemLog();
    ~SystemLog() = default;
    SystemLog(SystemLog const&) = delete;
    SystemLog(SystemLog&&) = delete;
    SystemLog& operator=(SystemLog const&) = delete;
    SystemLog& operator=(SystemLog&&) = delete;

public:
    static SystemLog* instance();

    void InitSystemLogger();

    template<typename Format, typename... Args>
    void outSys(uint8 logLevel, Format&& fmt, Args&& ... args)
    {
        outSys(logLevel, Warhead::StringFormat(std::forward<Format>(fmt), std::forward<Args>(args)...));
    }

private:
    void outSys(uint8 logLevel, std::string&& message);
};

#define sSysLog SystemLog::instance()

#define SYS_LOG_EXCEPTION_FREE(level__, ...) \
    { \
        try \
        { \
            sSysLog->outSys(level__, __VA_ARGS__); \
        } \
        catch (std::exception& e) \
        { \
            sSysLog->outSys(3, "Wrong format occurred (%s) at %s:%u.", \
                e.what(), __FILE__, __LINE__); \
        } \
    }

#if WH_PLATFORM != WH_PLATFORM_WINDOWS
void _check_args(char const*, ...) ATTR_PRINTF(1, 2);
void _check_args(std::string const&, ...);

// This will catch format errors on build time
#define SYS_LOG_MSG_BODY(level__, ...)                    \
        do                                                \
        {                                                 \
            if (false)                                    \
                _check_args(__VA_ARGS__);                 \
                                                          \
            SYS_LOG_EXCEPTION_FREE(level__, __VA_ARGS__); \
        } while (0)
#else
#define SYS_LOG_MSG_BODY(level__, ...)                    \
        __pragma(warning(push))                           \
        __pragma(warning(disable:4127))                   \
        do                                                \
        {                                                 \
            SYS_LOG_EXCEPTION_FREE(level__, __VA_ARGS__); \
        } while (0)                                       \
        __pragma(warning(pop))
#endif

// System Error level 3
#define SYS_LOG_ERROR(...) \
    SYS_LOG_MSG_BODY(3, __VA_ARGS__)

// System Info level 6
#define SYS_LOG_INFO(...) \
    SYS_LOG_MSG_BODY(6, __VA_ARGS__)

#endif
