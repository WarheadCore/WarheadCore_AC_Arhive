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

#ifndef _IP_NETWORK_H_
#define _IP_NETWORK_H_

#include "Define.h"
#include "IpAddress.h"
#include <asio/ip/network_v4.hpp>
#include <asio/ip/network_v6.hpp>

namespace warhead
{
    namespace Net
    {
        inline bool IsInNetwork(asio::ip::address_v4 const& networkAddress, asio::ip::address_v4 const& mask, asio::ip::address_v4 const& clientAddress)
        {
            asio::ip::network_v4 network = asio::ip::make_network_v4(networkAddress, mask);
            asio::ip::address_v4_range hosts = network.hosts();
            return hosts.find(clientAddress) != hosts.end();
        }

        inline asio::ip::address_v4 GetDefaultNetmaskV4(asio::ip::address_v4 const& networkAddress)
        {
            if ((address_to_uint(networkAddress) & 0x80000000) == 0)
                return asio::ip::address_v4(0xFF000000);

            if ((address_to_uint(networkAddress) & 0xC0000000) == 0x80000000)
                return asio::ip::address_v4(0xFFFF0000);

            if ((address_to_uint(networkAddress) & 0xE0000000) == 0xC0000000)
                return asio::ip::address_v4(0xFFFFFF00);

            return asio::ip::address_v4(0xFFFFFFFF);
        }

        inline bool IsInNetwork(asio::ip::address_v6 const& networkAddress, uint16 prefixLength, asio::ip::address_v6 const& clientAddress)
        {
            asio::ip::network_v6 network = asio::ip::make_network_v6(networkAddress, prefixLength);
            asio::ip::address_v6_range hosts = network.hosts();
            return hosts.find(clientAddress) != hosts.end();
        }
    }
}

#endif // _IP_NETWORK_H_
