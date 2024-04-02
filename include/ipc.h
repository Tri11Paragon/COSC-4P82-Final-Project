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
    EXECUTE_RUN,    //  Server -> Client    NumOfRuns
    CHILD_FIT,      //  Client -> Server    Fitness of Child
    PRUNE,          //  Server -> Client    NONE, Child should terminate
    // avg fitness, best fitness, avg tree size
    // unused
    AVG_FIT,        //  Client -> Server    Average Fitness, gen #
    BEST_FIT,       //  Client -> Server    Best fitness, gen #
    AVG_TREE,       //  Client -> Server    Avg Tree Size, gen #
};

struct packet_t
{
    state_t state;
    packet_id id;
    union
    {
        double fitness;
        union
        {
            struct
            {
                blt::i32 numOfGens;
                blt::i32 generation;
            };
            blt::u64 clock_time;
        };
    };
};

#endif //FINALPROJECT_RUNNER_STATES_H
