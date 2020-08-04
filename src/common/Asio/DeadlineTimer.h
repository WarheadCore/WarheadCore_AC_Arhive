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

#ifndef _DEAD_LINE_TIMER_H_
#define _DEAD_LINE_TIMER_H_

#include <asio/deadline_timer.hpp>

#define DeadlineTimerBase asio::basic_deadline_timer<boost::posix_time::ptime, asio::time_traits<boost::posix_time::ptime> , asio::io_context::executor_type>

namespace warhead
{
    namespace Asio
    {
        class DeadlineTimer : public DeadlineTimerBase
        {
        public:
            using DeadlineTimerBase::basic_deadline_timer;
        };
    }
}

#endif // _DEAD_LINE_TIMER_H_
