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

#ifndef ACORE_PACKETLOG_H
#define ACORE_PACKETLOG_H

#include "Common.h"

enum Direction
{
    CLIENT_TO_SERVER,
    SERVER_TO_CLIENT
};

class WorldPacket;

class PacketLog
{
private:
    PacketLog();
    ~PacketLog();

public:
    static PacketLog* instance();

    void Initialize();
    bool CanLogPacket() const { return (_file != nullptr); }
    void LogPacket(WorldPacket const& packet, Direction direction);

private:
    FILE* _file;
};

#define sPacketLog PacketLog::instance()

#endif
