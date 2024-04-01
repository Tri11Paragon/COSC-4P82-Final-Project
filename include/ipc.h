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
};

enum class packet_id : blt::u8
{
    // NAME,            DIRECTION           PAYLOAD
    EXECUTE_RUN,    //  Server -> Client    NumOfRuns
//    RUNS_COMPLETE,  //  Client -> Server    NONE
//    CHILD_EVAL,     //  Server -> Client    NONE (program should send out its fitness and wait)
    CHILD_FIT,      //  Client -> Server    Fitness of Child
    PRUNE,          //  Server -> Child     Fitness of children to prune. Child should exit if it doesn't meet this
};

struct packet_t
{
    state_t state;
    packet_id id;
    union
    {
        double fitness;
        blt::i32 numOfGens;
    };
};

#endif //FINALPROJECT_RUNNER_STATES_H
