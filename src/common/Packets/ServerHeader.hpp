#ifndef __SERVERHEADER_HPP
#define __SERVERHEADER_HPP

#pragma once

#include "Common.h"

#include "Log.h"

namespace default
{
    #if defined(__GNUC__)
        #pragma pack(1)
    #else
        #pragma pack(push, 1)
    #endif

    struct ServerPktHeader
    {
        ServerPktHeader(uint32 size, uint16 cmd) : m_size(size)
        {
            uint8 headerIndex = 0;

            header[headerIndex++] = 0xFF & (m_size>>24);
            header[headerIndex++] = 0xFF & (m_size>>16);
            header[headerIndex++] = 0xFF & (m_size>>8);
            header[headerIndex++] = 0xFF & m_size;

            header[headerIndex++] = 0xFF & cmd;
            header[headerIndex++] = 0xFF & (cmd>>8);
        }

        uint8 getHeaderLength()
        {
            // cmd = 2 bytes, size= 4 bytes
            return 6;
        }

        const uint32 m_size;
        uint8 header[6];
    };

    struct ClientPktHeader
    {
        uint32 size;
        uint16 cmd;
    };

    #if defined(__GNUC__)
        #pragma pack()
    #else
        #pragma pack(pop)
    #endif

}

#endif
