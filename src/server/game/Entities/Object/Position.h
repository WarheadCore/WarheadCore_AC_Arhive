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

#ifndef _POSITION_H_
#define _POSITION_H_

#include "Define.h"
#include <string>
#include <cmath>

class ByteBuffer;

struct Position
{
    Position(float x = 0, float y = 0, float z = 0, float o = 0)
        : m_positionX(x), m_positionY(y), m_positionZ(z), m_orientation(NormalizeOrientation(o)) { }

    Position(Position const& loc) { Relocate(loc); }

    /* requried as of C++ 11 */
#if __cplusplus >= 201103L
    Position(Position&&) = default;
    Position& operator=(const Position&) = default;
    Position& operator=(Position&&) = default;
#endif

    struct PositionXYStreamer
    {
        explicit PositionXYStreamer(Position& pos) : Pos(&pos) { }
        Position* Pos;
    };

    struct PositionXYZStreamer
    {
        explicit PositionXYZStreamer(Position& pos) : m_pos(&pos) {}
        Position* m_pos;
    };

    struct PositionXYZOStreamer
    {
        explicit PositionXYZOStreamer(Position& pos) : m_pos(&pos) {}
        Position* m_pos;
    };

    float m_positionX = 0;
    float m_positionY = 0;
    float m_positionZ = 0;
    float m_orientation = 0;

    bool operator==(Position const& a);

    inline bool operator!=(Position const& a)
    {
        return !(operator==(a));
    }

    void Relocate(float x, float y)
    {
        m_positionX = x;
        m_positionY = y;
    }

    void Relocate(float x, float y, float z)
    {
        m_positionX = x;
        m_positionY = y;
        m_positionZ = z;
    }

    void Relocate(float x, float y, float z, float orientation)
    {
        m_positionX = x;
        m_positionY = y;
        m_positionZ = z;
        m_orientation = orientation;
    }

    void Relocate(const Position& pos)
    {
        m_positionX = pos.m_positionX;
        m_positionY = pos.m_positionY;
        m_positionZ = pos.m_positionZ;
        m_orientation = pos.m_orientation;
    }

    void Relocate(const Position* pos)
    {
        m_positionX = pos->m_positionX;
        m_positionY = pos->m_positionY;
        m_positionZ = pos->m_positionZ;
        m_orientation = pos->m_orientation;
    }

    void RelocatePolarOffset(float angle, float dist, float z = 0.0f);

    void RelocateOffset(const Position& offset);

    void SetOrientation(float orientation)
    {
        m_orientation = orientation;
    }

    float GetPositionX() const { return m_positionX; }
    float GetPositionY() const { return m_positionY; }
    float GetPositionZ() const { return m_positionZ; }
    float GetOrientation() const { return m_orientation; }

    void GetPosition(float& x, float& y) const
    {
        x = m_positionX;
        y = m_positionY;
    }

    void GetPosition(float& x, float& y, float& z) const
    {
        x = m_positionX;
        y = m_positionY;
        z = m_positionZ;
    }

    void GetPosition(float& x, float& y, float& z, float& o) const
    {
        x = m_positionX;
        y = m_positionY;
        z = m_positionZ;
        o = m_orientation;
    }

    void GetPosition(Position* pos) const
    {
        if (pos)
            pos->Relocate(m_positionX, m_positionY, m_positionZ, m_orientation);
    }

    Position GetPosition() const { return *this; }

    Position::PositionXYZStreamer PositionXYZStream()
    {
        return PositionXYZStreamer(*this);
    }

    Position::PositionXYZOStreamer PositionXYZOStream()
    {
        return PositionXYZOStreamer(*this);
    }

    bool IsPositionValid() const;

    float GetExactDist2dSq(const float x, const float y) const
    {
        float dx = x - m_positionX;
        float dy = y - m_positionY;
        return dx*dx + dy*dy;
    }
    float GetExactDist2dSq(Position const& pos) const { return GetExactDist2dSq(pos.m_positionX, pos.m_positionY); }
    float GetExactDist2dSq(Position const* pos) const { return GetExactDist2dSq(*pos); }
    float GetExactDist2d(const float x, const float y) const { return std::sqrt(GetExactDist2dSq(x, y)); }
    float GetExactDist2d(Position const& pos) const { return GetExactDist2d(pos.m_positionX, pos.m_positionY); }
    float GetExactDist2d(Position const* pos) const { return GetExactDist2d(*pos); }
    float GetExactDistSq(float x, float y, float z) const
    {
        float dz = z - m_positionZ;
        return GetExactDist2dSq(x, y) + dz*dz;
    }
    float GetExactDistSq(Position const& pos) const { return GetExactDistSq(pos.m_positionX, pos.m_positionY, pos.m_positionZ); }
    float GetExactDistSq(Position const* pos) const { return GetExactDistSq(*pos); }
    float GetExactDist(float x, float y, float z) const { return std::sqrt(GetExactDistSq(x, y, z)); }
    float GetExactDist(Position const& pos) const { return GetExactDist(pos.m_positionX, pos.m_positionY, pos.m_positionZ); }
    float GetExactDist(Position const* pos) const { return GetExactDist(*pos); }

    void GetPositionOffsetTo(const Position& endPos, Position& retOffset) const;
    Position GetPositionWithOffset(Position const& offset) const;

    float GetAngle(const Position* pos) const;
    float GetAngle(float x, float y) const;

    float GetRelativeAngle(const Position* pos) const
    {
        return GetAngle(pos) - m_orientation;
    }

    float GetRelativeAngle(float x, float y) const { return GetAngle(x, y) - m_orientation; }

    void GetSinCos(float x, float y, float& vsin, float& vcos) const;

    bool IsInDist2d(float x, float y, float dist) const
    {
        return GetExactDist2dSq(x, y) < dist * dist;
    }

    bool IsInDist2d(const Position* pos, float dist) const
    {
        return GetExactDist2dSq(pos) < dist * dist;
    }

    bool IsInDist(float x, float y, float z, float dist) const
    {
        return GetExactDistSq(x, y, z) < dist * dist;
    }

    bool IsInDist(const Position* pos, float dist) const
    {
        return GetExactDistSq(pos) < dist * dist;
    }

    bool IsWithinBox(const Position& center, float xradius, float yradius, float zradius) const;
    bool HasInArc(float arcangle, const Position* pos, float targetRadius = 0.0f) const;
    bool HasInLine(Position const* pos, float width) const;
    std::string ToString() const;

    // modulos a radian orientation to the range of 0..2PI
    static float NormalizeOrientation(float o)
    {
        // fmod only supports positive numbers. Thus we have
        // to emulate negative numbers
        if (o < 0)
        {
            float mod = o * -1;
            mod = fmod(mod, 2.0f * static_cast<float>(M_PI));
            mod = -mod + 2.0f * static_cast<float>(M_PI);
            return mod;
        }
        return fmod(o, 2.0f * static_cast<float>(M_PI));
    }
};

#define MAPID_INVALID 0xFFFFFFFF

class WorldLocation : public Position
{
    public:
        explicit WorldLocation(uint32 _mapId = MAPID_INVALID, float x = 0.f, float y = 0.f, float z = 0.f, float o = 0.f)
            : Position(x, y, z, o), m_mapId(_mapId) { }

        WorldLocation(uint32 mapId, Position const& position)
            : Position(position), m_mapId(mapId) { }

        void WorldRelocate(WorldLocation const& loc) { m_mapId = loc.GetMapId(); Relocate(loc); }
        void WorldRelocate(WorldLocation const* loc) { m_mapId = loc->GetMapId(); Relocate(loc); }
        void WorldRelocate(uint32 mapId, Position const& pos) { m_mapId = mapId; Relocate(pos); }
        void WorldRelocate(uint32 mapId = MAPID_INVALID, float x = 0.f, float y = 0.f, float z = 0.f, float o = 0.f)
        {
            m_mapId = mapId;
            Relocate(x, y, z, o);
        }

        WorldLocation GetWorldLocation() const
        {
            return *this;
        }

        uint32 GetMapId() const { return m_mapId; }

        uint32 m_mapId;
};

ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYStreamer const& streamer);
ByteBuffer& operator >> (ByteBuffer& buf, Position::PositionXYStreamer const& streamer);
ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYZStreamer const& streamer);
ByteBuffer& operator >> (ByteBuffer& buf, Position::PositionXYZStreamer const& streamer);
ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYZOStreamer const& streamer);
ByteBuffer& operator >> (ByteBuffer& buf, Position::PositionXYZOStreamer const& streamer);

#endif // _POSITION_H_
