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

#ifndef _RESOLVER_H_
#define _RESOLVER_H_

#include <asio/ip/tcp.hpp>
#include <string>
#include <optional>

namespace warhead
{
    namespace Net
    {
        inline std::optional<asio::ip::tcp::endpoint> Resolve(asio::ip::tcp::resolver& resolver, asio::ip::tcp const& protocol,
            std::string const& host, std::string const& service)
        {
            std::error_code ec;
            asio::ip::resolver_base::flags flagsResolver = asio::ip::resolver_base::all_matching;
            asio::ip::tcp::resolver::results_type results = resolver.resolve(protocol, host, service, flagsResolver, ec);

            if (results.begin() == results.end() || ec)
                return {};

            return results.begin()->endpoint();
        }
    }
}

#endif // _RESOLVER_H_
