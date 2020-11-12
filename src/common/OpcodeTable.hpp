#ifndef __OPCODETABLE_HPP
#define __OPCODETABLE_HPP

#pragma once

#include "Common.h"
#include "OpcodeDefines.hpp"
#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"

OpcodeTable* OpcodeTable::instance()
{
    static OpcodeTable instance;
    return &instance;
}

enum SessionStatus
{
    STATUS_AUTHED = 0,                      // Player authenticated (_player == NULL, m_playerRecentlyLogout = false or will be reset before handler call, m_GUID have garbage)
    STATUS_LOGGEDIN,                        // Player in game (_player != NULL, m_GUID == _player->GetGUID(), inWorld())
    STATUS_TRANSFER,                        // Player transferring to another map (_player != NULL, m_GUID == _player->GetGUID(), !inWorld())
    STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT,    // _player!= NULL or _player == NULL && m_playerRecentlyLogout, m_GUID store last _player guid)
    STATUS_NEVER,                           // Opcode not accepted from client (deprecated or server side only)
    STATUS_UNHANDLED,                       // Opcode not handled yet
};

enum PacketState
{
    PACKET_STATE_NULL = 0,
    PACKET_STATE_HANDLE = 2 << 0,
    PACKET_STATE_PASS = 2 << 1,
    PACKET_STATE_BOTH = PACKET_STATE_HANDLE | PACKET_STATE_PASS
};

struct OpcodeHandler
{
    char const* name;
    uint16 opcode;

    bool multithreaded;
    SessionStatus status;
    PacketState toNode;
    PacketState fromNode;
};

class OpcodeTable
{
public:
    static OpcodeTable* instance();

    OpcodeHandler const* GetOpcodeHandler(uint32 index) const
    {
        return _opcodes[index];
    }

    bool IsMultiThreadedOpcode(uint32 index) const
    {
        if (index < NUM_MSG_TYPES)
            return _opcodes[index]->multithreaded;

        return false;
    }
    void ValidateAndSetOpcode(bool isInValidRange, bool isNonZero, OpcodeHandler *opcodeHandler)
    {
        if (isInValidRange && isNonZero)
        {
            if (_opcodes[opcodeHandler->opcode] != nullptr)
            {
                log("Tried to override handler of %s with %s (opcode %u)", _opcodes[opcodeHandler->opcode]->name, opcodeHandler->name, opcodeHandler->opcode);
                delete opcodeHandler;
                return;
            }

            _opcodes[opcodeHandler->opcode] = opcodeHandler;
        }
        else if (!isInValidRange && isNonZero)
        {
           log("Tried to set handler for an invalid opcode %d", opcodeHandler->opcode);
            delete opcodeHandler;
        }
        else if (isInValidRange && !isNonZero)
        {
            log("Opcode %s got value 0", opcodeHandler->name);
            delete opcodeHandler;
        }
        else
            log("OpcodeTable: Something went horribly wrong");
    }

private:
    // Prevent copying this structure
    OpcodeTable() = default;
    OpcodeTable(OpcodeTable const&) = delete;
    OpcodeTable& operator=(OpcodeTable const&) = delete;
    ~OpcodeTable() = default;

    OpcodeTable()
    {
        memset(_opcodes, 0, sizeof(_opcodes));
    }

    ~OpcodeTable()
    {
        for (uint32 i = 0; i < NUM_MSG_TYPES; ++i)
            if (_opcodes[i])
                delete _opcodes[i];
    }

    OpcodeHandler* _opcodes[NUM_MSG_TYPES];
};

#define sOpcodeTable OpcodeTable::instance()

#endif /* __OPCODETABLE_HPP */
