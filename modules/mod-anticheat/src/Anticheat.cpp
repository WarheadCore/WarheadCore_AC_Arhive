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

#include "Anticheat.h"
#include "Log.h"
#include "GameConfig.h"
#include "StringFormat.h"

#define CLIMB_ANGLE 1.9f

void AnticheatData::SetLastOpcode(uint32 opcode)
{
    lastOpcode = opcode;
}

void AnticheatData::SetPosition(float x, float y, float z, float o)
{
    lastMovementInfo.pos = { x, y, z, o };
}

uint32 AnticheatData::GetLastOpcode() const
{
    return lastOpcode;
}

const MovementInfo& AnticheatData::GetLastMovementInfo() const
{
    return lastMovementInfo;
}

void AnticheatData::SetLastMovementInfo(MovementInfo& moveInfo)
{
    lastMovementInfo = moveInfo;
}

/// Mgr
Anticheat::Anticheat()
{
}

Anticheat::~Anticheat()
{
    m_Players.clear();
}

Anticheat* Anticheat::instance()
{
    static Anticheat instance;
    return &instance;
}

void Anticheat::JumpHackDetection(Player* player, MovementInfo /* movementInfo */, uint32 opcode)
{
    if (!CONF_GET_BOOL("AC.Detect.JumpHack.Enable"))
        return;

    uint32 key = player->GetGUIDLow();

    if (m_Players[key].GetLastOpcode() == MSG_MOVE_JUMP && opcode == MSG_MOVE_JUMP)
    {
        LOG_INFO("modules", "> Anticheat: Jump-Hack detected player %s (%u)", player->GetName().c_str(), player->GetGUIDLow());
        BuildReport(player);
    }
}

void Anticheat::WalkOnWaterHackDetection(Player* player, MovementInfo  movementInfo)
{
    if (!CONF_GET_BOOL("AC.Detect.WaterWalkHack.Enable"))
        return;

    uint32 key = player->GetGUIDLow();
    /* Thanks to @LilleCarl */
    if (!m_Players[key].GetLastMovementInfo().HasMovementFlag(MOVEMENTFLAG_WATERWALKING) && !movementInfo.HasMovementFlag(MOVEMENTFLAG_WATERWALKING))
        return;

    // if we are a ghost we can walk on water
    if (!player->IsAlive())
        return;

    if (player->HasAuraType(SPELL_AURA_FEATHER_FALL) ||
        player->HasAuraType(SPELL_AURA_SAFE_FALL) ||
        player->HasAuraType(SPELL_AURA_WATER_WALK))
        return;

    LOG_INFO("modules", "> Anticheat: Walk on Water - Hack detected player %s (%u)", player->GetName().c_str(), player->GetGUIDLow());

    BuildReport(player);
}

void Anticheat::FlyHackDetection(Player* player, MovementInfo  movementInfo)
{
    if (!CONF_GET_BOOL("AC.Detect.FlyHack.Enable"))
        return;

    uint32 key = player->GetGUIDLow();

    if (player->HasAuraType(SPELL_AURA_FLY) || player->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) || player->HasAuraType(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED))//overkill but wth
        return;

    /*Thanks to @LilleCarl for info to check extra flag*/
    bool stricterChecks = true;

    /*if (CONF_GET_BOOL("Anticheat.StricterFlyHackCheck"))
        stricterChecks = !(movementInfo.HasMovementFlag(MOVEMENTFLAG_ASCENDING) && !player->IsInWater());*/

    if (!movementInfo.HasMovementFlag(MOVEMENTFLAG_CAN_FLY) && !movementInfo.HasMovementFlag(MOVEMENTFLAG_FLYING) && stricterChecks)
        return;

    LOG_INFO("modules", "> Anticheat: Fly-Hack detected player %s (%u)", player->GetName().c_str(), player->GetGUIDLow());

    BuildReport(player);
}

void Anticheat::TeleportPlaneHackDetection(Player* player, MovementInfo movementInfo)
{
    if (!CONF_GET_BOOL("AC.Detect.TelePlaneHack.Enable"))
        return;

    uint32 key = player->GetGUIDLow();

    if (m_Players[key].GetLastMovementInfo().pos.GetPositionZ() != 0 ||
        movementInfo.pos.GetPositionZ() != 0)
        return;

    if (movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING))
        return;

    float x, y, z;
    player->GetPosition(x, y, z);
    float ground_Z = player->GetMap()->GetHeight(x, y, z);
    float z_diff = fabs(ground_Z - z);

    // we are not really walking there
    if (z_diff > 1.0f)
    {
        LOG_INFO("modules", "AnticheatMgr:: Teleport To Plane - Hack detected player %s (%u)", player->GetName().c_str(), player->GetGUIDLow());
        BuildReport(player);
    }
}

void Anticheat::StartHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode)
{
    if (!CONF_GET_BOOL("AC.Enable"))
        return;

    if (player->IsGameMaster())
        return;

    uint32 key = player->GetGUIDLow();

    if (player->IsInFlight() || player->GetTransport() || player->GetVehicle())
    {
        m_Players[key].SetLastMovementInfo(movementInfo);
        m_Players[key].SetLastOpcode(opcode);
        return;
    }

    SpeedHackDetection(player, movementInfo);
    FlyHackDetection(player, movementInfo);
    WalkOnWaterHackDetection(player, movementInfo);
    JumpHackDetection(player, movementInfo, opcode);
    TeleportPlaneHackDetection(player, movementInfo);
    ClimbHackDetection(player, movementInfo, opcode);

    m_Players[key].SetLastMovementInfo(movementInfo);
    m_Players[key].SetLastOpcode(opcode);
}

// basic detection
void Anticheat::ClimbHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode)
{
    if (!CONF_GET_BOOL("AC.Detect.ClimbHack.Enable"))
        return;

    uint32 key = player->GetGUIDLow();

    if (opcode != MSG_MOVE_HEARTBEAT || m_Players[key].GetLastOpcode() != MSG_MOVE_HEARTBEAT)
        return;

    // in this case we don't care if they are "legal" flags, they are handled in another parts of the Anticheat Manager.
    if (player->IsInWater() || player->IsFlying() || player->IsFalling())
        return;

    Position playerPos;

    float deltaZ = fabs(playerPos.GetPositionZ() - movementInfo.pos.GetPositionZ());
    float deltaXY = movementInfo.pos.GetExactDist2d(&playerPos);
    float angle = Position::NormalizeOrientation(tan(deltaZ / deltaXY));

    if (angle > CLIMB_ANGLE)
    {
        LOG_INFO("modules", "> Anticheat: Climb-Hack detected player %s (%u)", player->GetName().c_str(), player->GetGUIDLow());
        BuildReport(player);
    }
}

void Anticheat::SpeedHackDetection(Player* player, MovementInfo movementInfo)
{
    if (!CONF_GET_BOOL("AC.Detect.SpeedHack.Enable"))
        return;

    uint32 key = player->GetGUIDLow();

    // We also must check the map because the movementFlag can be modified by the client.
    // If we just check the flag, they could always add that flag and always skip the speed hacking detection.
    // 369 == DEEPRUN TRAM
    if (m_Players[key].GetLastMovementInfo().HasMovementFlag(MOVEMENTFLAG_ONTRANSPORT) && player->GetMapId() == 369)
        return;

    uint32 distance2D = (uint32)movementInfo.pos.GetExactDist2d(&m_Players[key].GetLastMovementInfo().pos);
    uint8 moveType = 0;

    // we need to know HOW is the player moving
    // TO-DO: Should we check the incoming movement flags?
    if (player->HasUnitMovementFlag(MOVEMENTFLAG_SWIMMING))
        moveType = MOVE_SWIM;
    else if (player->IsFlying())
        moveType = MOVE_FLIGHT;
    else if (player->HasUnitMovementFlag(MOVEMENTFLAG_WALKING))
        moveType = MOVE_WALK;
    else
        moveType = MOVE_RUN;

    // how many yards the player can do in one sec.
    uint32 speedRate = (uint32)(player->GetSpeed(UnitMoveType(moveType)) + movementInfo.jump.xyspeed);

    // how long the player took to move to here.
    uint32 timeDiff = getMSTimeDiff(m_Players[key].GetLastMovementInfo().time, movementInfo.time);

    if (!timeDiff)
        timeDiff = 1;

    // this is the distance doable by the player in 1 sec, using the time done to move to this point.
    uint32 clientSpeedRate = distance2D * 1000 / timeDiff;

    // we did the (uint32) cast to accept a margin of tolerance
    if (clientSpeedRate > speedRate)
    {
        BuildReport(player);
        LOG_INFO("modules", "> Anticheat: Speed-Hack detected player %s (%u)", player->GetName().c_str(), player->GetGUIDLow());
    }
}

void Anticheat::HandlePlayerLogin(Player* player)
{
    if (!player)
        return;

    // we initialize the pos of lastMovementPosition.
    m_Players[player->GetGUIDLow()].SetPosition(player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation());
}

void Anticheat::HandlePlayerLogout(Player* player)
{
    if (!player)
        return;

    m_Players.erase(player->GetGUIDLow());
}

void Anticheat::BuildReport(Player* player)
{
    if (!CONF_GET_BOOL("AC.Info.Enable"))
        return;

    uint32 key = player->GetGUIDLow();

    // display warning at the center of the screen, hacky way?
    std::string str = warhead::StringFormat("[%s] использует читы!", player->GetName().c_str());
    WorldPacket data(SMSG_NOTIFICATION, (str.size() + 1));
    data << str;
    sWorld->SendGlobalGMMessage(&data);
}
