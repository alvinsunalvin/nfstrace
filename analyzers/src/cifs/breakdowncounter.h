//------------------------------------------------------------------------------
// Author: Andrey Kuznetsov
// Description: Statistics counter
// Copyright (c) 2015 EPAM Systems
//------------------------------------------------------------------------------
/*
    This file is part of Nfstrace.

    Nfstrace is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 2 of the License.

    Nfstrace is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Nfstrace.  If not, see <http://www.gnu.org/licenses/>.
*/

//------------------------------------------------------------------------------
#ifndef BREAKDOWNCOUNTER_H
#define BREAKDOWNCOUNTER_H
//------------------------------------------------------------------------------
#include <map>

#include "latencies.h"
//------------------------------------------------------------------------------
/*! Counts and keeps breakdown statistics for session
 */
class BreakdownCounter
{
public:
    BreakdownCounter();
    ~BreakdownCounter();

    /*!
     * \brief operator [] returns statistics by index (command number)
     * \param index - command number
     * \return statistics
     */
    const NST::breakdown::Latencies operator[](int index) const;

    /*!
     * \brief operator [] returns statistics by index (command number)
     * \param index - command number
     * \return statistics
     */
    NST::breakdown::Latencies& operator[](int index);

    /*!
     * \brief get_total_count returns total amount of commands
     * \return commands count
     */
    uint64_t get_total_count () const;

private:
    void operator= (const BreakdownCounter&) = delete;
    std::map<int, NST::breakdown::Latencies> latencies;
};

/*!
 * Saves statistics on commands receive
 * \param proc - command
 * \param cmd_code - commands code
 * \param stats - statistics
 */
template<typename Cmd, typename Code, typename Stats>
void account(const Cmd* proc, Code cmd_code, Stats& stats)
{
    timeval latency {0, 0};

    // diff between 'reply' and 'call' timestamps
    timersub(proc->rtimestamp, proc->ctimestamp, &latency);

    ++stats.procedures_total_count;
    ++stats.procedures_count[static_cast<int>(cmd_code)];

    auto i = stats.per_procedure_statistics.find(*proc->session);
    if (i == stats.per_procedure_statistics.end())
    {
        auto session_res = stats.per_procedure_statistics.emplace(*proc->session, BreakdownCounter {});
        if (session_res.second == false)
        {
            return;
        }
        i = session_res.first;
    }

    (i->second)[static_cast<int>(cmd_code)].add(latency);
}
//------------------------------------------------------------------------------
#endif // BREAKDOWNCOUNTER_H
//------------------------------------------------------------------------------
