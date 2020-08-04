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

#ifndef _IO_CONTEXT_H_
#define _IO_CONTEXT_H_

#include <asio/io_context.hpp>
#include <asio/post.hpp>

namespace warhead
{
    namespace Asio
    {
        class IoContext
        {
        public:
            IoContext() : _impl() { }
            explicit IoContext(int concurrency_hint) : _impl(concurrency_hint) { }

            operator asio::io_context&() { return _impl; }
            operator asio::io_context const&() const { return _impl; }

            std::size_t run() { return _impl.run(); }
            void stop() { _impl.stop(); }

            asio::io_context::executor_type get_executor() noexcept { return _impl.get_executor(); }

        private:
            asio::io_context _impl;
        };

        template<typename T>
        inline decltype(auto) post(asio::io_context& ioContext, T&& t)
        {
            return asio::post(ioContext, std::forward<T>(t));
        }

        template<typename T>
        inline decltype(auto) get_io_context(T&& ioObject)
        {
            return ioObject.get_executor().context();
        }
    }
}

#endif // _IO_CONTEXT_H_
