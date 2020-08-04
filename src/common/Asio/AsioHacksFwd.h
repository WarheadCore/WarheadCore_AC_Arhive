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

#ifndef _ASIO_HACKS_FWD_H_
#define _ASIO_HACKS_FWD_H_

/**
  Collection of forward declarations to improve compile time
 */
namespace boost
{
    namespace posix_time
    {
        class ptime;
    }

    namespace asio
    {
        template <typename Time>
        struct time_traits;

        namespace ip
        {
            class address;

            class tcp;

            template <typename InternetProtocol>
            class basic_endpoint;

            typedef basic_endpoint<tcp> tcp_endpoint;
        }

        class executor;

        namespace ip
        {
            template <typename InternetProtocol, typename Executor>
            class basic_resolver;

            typedef basic_resolver<tcp, executor> tcp_resolver;
        }
    }
}

namespace warhead
{
    namespace Asio
    {
        class Strand;
    }
}

#endif // _ASIO_HACKS_FWD_H_
