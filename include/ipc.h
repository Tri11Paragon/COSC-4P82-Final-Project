#pragma once
/*
 *  Copyright (C) 2024  Brett Terpstra
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef FINALPROJECT_RUNNER_STATES_H
#define FINALPROJECT_RUNNER_STATES_H

#include <blt/std/types.h>
#include "blt/std/logging.h"

// use a state-machine to control what phase of the pyramid search we are in.
enum class state_t : blt::u8
{
    // NAME,
    RUN_GENERATIONS,
    CHILD_EVALUATION,
    PRUNE,
    IDLE,
};

enum class packet_id : blt::u8
{
    // NAME,            DIRECTION           PAYLOAD
    EXECUTE_RUN,//  Server -> Client    numOfGens,  Number of runs to execute
    CHILD_FIT,  //  Client -> Server    fitness,    Fitness of Child
    PRUNE,      //  Server -> Client    NONE,       Child should terminate
    EXEC_TIME,  //  Client -> Server    timer,      wall time in ms
    CPU_TIME,   //  Client -> Server    timer,      cpu time in ms
    CPU_CYCLES, //  Client -> Server    timer,      number of cpu cycles used
    MEM_USAGE,  //  Client -> Server    timer,      memory usage
    // avg fitness, best fitness, avg tree size
    // unused
    AVG_FIT, //  Client -> Server    Average Fitness, gen #
    BEST_FIT,//  Client -> Server    Best fitness, gen #
    AVG_TREE,//  Client -> Server    Avg Tree Size, gen #
};

struct memory_snapshot
{
    blt::u64 memory;
    blt::u64 timeSinceStart;
};

struct process_info_t
{
    blt::u64 wall_time = 0;
    blt::u64 cpu_time = 0;
    blt::u64 cpu_cycles = 0;
    std::vector<memory_snapshot> snapshots;
    
    process_info_t& operator+=(const process_info_t& info)
    {
        if (info.snapshots.size() == snapshots.size())
        {
            for (const auto& v : blt::enumerate(info.snapshots))
            {
                snapshots[v.first].memory += v.second.memory;
                snapshots[v.first].timeSinceStart += v.second.timeSinceStart;
            }
        } else
            BLT_WARN("Hey maybe you should fix your stupid program!");
        wall_time += info.wall_time;
        cpu_time += info.cpu_time;
        cpu_cycles += info.cpu_cycles;
        return *this;
    }
    
    process_info_t& operator/=(int i)
    {
        for (auto& v : snapshots)
        {
            v.memory /= i;
            v.timeSinceStart /= i;
        }
        wall_time /= i;
        cpu_time /= i;
        cpu_cycles /= i;
        
        return *this;
    }
};

struct packet_t
{
    state_t state;
    packet_id id;
    union
    {
        double fitness;
        blt::u64 timer;
        memory_snapshot snapshot;
        blt::i32 numOfGens;
    };
};

#endif//FINALPROJECT_RUNNER_STATES_H
