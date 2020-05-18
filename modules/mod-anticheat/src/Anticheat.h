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

#ifndef _ANTICHEAT_H_
#define _ANTICHEAT_H_

#include "Common.h"
#include "Player.h"

class AnticheatData
{
public:
    void SetLastOpcode(uint32 opcode);
    uint32 GetLastOpcode() const;

    const MovementInfo& GetLastMovementInfo() const;
    void SetLastMovementInfo(MovementInfo& moveInfo);

    void SetPosition(float x, float y, float z, float o);

private:
    uint32 lastOpcode = 0;
    MovementInfo lastMovementInfo;
};

class Anticheat
{
    Anticheat();
    ~Anticheat();

public:
    static Anticheat* instance();

    void StartHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);
    void HandlePlayerLogin(Player* player);
    void HandlePlayerLogout(Player* player);

private:
    void SpeedHackDetection(Player* player, MovementInfo movementInfo);
    void FlyHackDetection(Player* player, MovementInfo movementInfo);
    void WalkOnWaterHackDetection(Player* player, MovementInfo movementInfo);
    void JumpHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);
    void TeleportPlaneHackDetection(Player* player, MovementInfo);
    void ClimbHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);

    void BuildReport(Player* player);

    // GUIDLow is the key.
    typedef std::map<uint32, AnticheatData> AnticheatPlayersDataMap;

    AnticheatPlayersDataMap m_Players;                        ///< Player data
};

#define sAnticheat Anticheat::instance()

#endif /* _ANTICHEAT_H_ */
