#ifndef __PACKETHEADER_HPP
#define __PACKETHEADER_HPP

#pragma once

#include "Common.h"

#include "Log.h"

namespace warhead
{
    #if defined(__GNUC__)
        #pragma pack(1)
    #else
        #pragma pack(push, 1)
    #endif


    struct ServerPktHeader
    {
        /**
         * size is the length of the payload _plus_ the length of the opcode
         */
        ServerPktHeader(uint32 size, uint16 cmd) : size(size)
        {
            uint8 headerIndex=0;
            if (isLargePacket())
            {
                header[headerIndex++] = 0x80|(0xFF &(size>>16));
            }
            header[headerIndex++] = 0xFF &(size>>8);
            header[headerIndex++] = 0xFF &size;

            header[headerIndex++] = 0xFF & cmd;
            header[headerIndex++] = 0xFF & (cmd>>8);
        }

        uint8 getHeaderLength()
        {
            // cmd = 2 bytes, size= 2||3bytes
            return 2+(isLargePacket()?3:2);
        }

        bool isLargePacket() const
        {
            return size > 0x7FFF;
        }

        const uint32 size;
        uint8 header[5];
    };

    struct ClientPktHeader
    {
        std::uint16_t size;
        std::uint32_t cmd;
    };

    #if defined(__GNUC__)
        #pragma pack()
    #else
        #pragma pack(pop)
    #endif
}

#endif
