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

#include "SystemLog.h"
#include "Config.h"
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/SplitterChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>

#if WH_PLATFORM == WH_PLATFORM_WINDOWS
#include <Poco/WindowsConsoleChannel.h>
#define CONSOLE_CHANNEL WindowsColorConsoleChannel
#else
#include <Poco/ConsoleChannel.h>
#define CONSOLE_CHANNEL ColorConsoleChannel
#endif

using namespace Poco;

namespace
{
    std::string const& LOGGER_SYSTEM = "system";
}

SystemLog::SystemLog()
{
    InitSystemLogger();
}

SystemLog* SystemLog::instance()
{
    static SystemLog instance;
    return &instance;
}

void SystemLog::InitSystemLogger()
{
    // Start console channel
    AutoPtr<PatternFormatter> _consolePattern(new PatternFormatter);

#define LOG_CATCH \
        catch (const std::exception& e) \
        { \
            printf("Log::InitSystemLogger - %s\n", e.what()); \
        } \

    try
    {
        _consolePattern->setProperty("pattern", "%H:%M:%S %t");
        _consolePattern->setProperty("times", "local");
    }
    LOG_CATCH

    AutoPtr<CONSOLE_CHANNEL> _consoleChannel(new CONSOLE_CHANNEL);

    try
    {
        _consoleChannel->setProperty("informationColor", "cyan");
    }
    LOG_CATCH

    // Start file channel
    AutoPtr<FileChannel> _fileChannel(new FileChannel);

    try
    {
        _fileChannel->setProperty("path", "System.log");
        _fileChannel->setProperty("times", "local");
        _fileChannel->setProperty("flush", "false");
    }
    LOG_CATCH

    AutoPtr<PatternFormatter> _filePattern(new PatternFormatter);

    try
    {
        _filePattern->setProperty("pattern", "%Y-%m-%d %H:%M:%S %t");
        _filePattern->setProperty("times", "local");
    }
    LOG_CATCH

    AutoPtr<SplitterChannel> _split(new SplitterChannel);

    try
    {
        _split->addChannel(new FormattingChannel(_consolePattern, _consoleChannel));
        _split->addChannel(new FormattingChannel(_filePattern, _fileChannel));
    }
    LOG_CATCH

    try
    {
        Logger::create(LOGGER_SYSTEM, _split, 6);
    }
    LOG_CATCH

#undef LOG_CATCH
}

void SystemLog::outSys(uint8 logLevel, std::string&& message)
{
    Logger& logger = Logger::get(LOGGER_SYSTEM);

    try
    {
        switch (logLevel)
        {
            case 3:
                logger.error(message);
                break;
            case 6:
                logger.information(message);
                break;
            default:
                break;
        }
    }
    catch (const std::exception& e)
    {
        printf("SystemLog::outSys - %s\n", e.what());
    }
}
