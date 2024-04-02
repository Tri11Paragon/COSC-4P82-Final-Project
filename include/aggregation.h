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

#include "blt/std/hashmap.h"
#include "blt/std/memory.h"
#ifndef FINALPROJECT_RUNNER_AGGREGATION_H
#define FINALPROJECT_RUNNER_AGGREGATION_H

#include <blt/std/types.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <type_traits>
#include <blt/std/logging.h>
#include <blt/std/utility.h>
#include <string_view>

template<typename T>
static inline void fill_value(T& v, const std::string& str)
{
    try
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            v = std::stod(str);
        } else if constexpr (std::is_integral_v<T>)
        {
            v = std::stoi(str);
        } else
        {
            static_assert("Unsupported type!");
        }
    } catch (const std::exception& e)
    {
        BLT_ERROR("Failed to convert value from string '%s' to type '%s' what(): %s", str.c_str(), blt::type_string<T>().c_str(), e.what());
    }
}

/**
 * Structure used to store information loaded from the .stt file
 */
struct stt_record
{
    public:
        stt_record() = default;
        
        int gen, sub;
        double mean_fitness;
        double best_fitness, worse_fitness, mean_tree_size, mean_tree_depth;
        int best_tree_size, best_tree_depth, worse_tree_size, worse_tree_depth;
        double mean_fitness_run;
        double best_fitness_run, worse_fitness_run, mean_tree_size_run, mean_tree_depth_run;
        int best_tree_size_run, best_tree_depth_run, worse_tree_size_run, worse_tree_depth_run;
        
        stt_record& operator+=(const stt_record& r);
        
        stt_record& operator/=(int i);
        
        static stt_record from_string_array(int generation, size_t& idx, blt::span<std::string> values);
};

/**
 * Structure used to store information loaded from the .fn file
 */
struct fn_record
{
    // real value = 'cammeo' predicted value = 'cammeo'
    blt::size_t cc = 0;
    // real value = 'cammeo' predicted value = 'osmancik'
    blt::size_t co = 0;
    // real value = 'osmancik' predicted value = 'osmancik'
    blt::size_t oo = 0;
    // real value = 'osmancik' predicted value = 'cammeo'
    blt::size_t oc = 0;
    double fitness = 0;
    // hits from the training data, kinda useless
    blt::size_t hits = 0;
};

struct runs_stt_data
{
    // per generation (map stores from gen -> list of rows (records))
    blt::hashmap_t<int, std::vector<stt_record>> averages;
    // per RUN generation size
    std::vector<int> runs_generation_size;
    // count of the number of generations that use the same number of generations (used for calculating mode) exists [0, largest_generation]
    blt::scoped_buffer<int> generations_size_count;
    // largest number of generations from all runs
    int largest_generation = 0;
    // total number of generations across all runs (r1...rn)
    int total_generations = 0;
    // total / runs
    int generations_average = 0;
    // # of run lengths value is `data.generations_size_count[data.mode_generation]`
    int generations_mode = 0;
    // the generation that is the mode
    int mode_generation = 0;
};

struct runs_fn_data
{
    std::vector<fn_record> runs;
    // index of the best recorded run
    blt::size_t best = 0;
    // total number of hits from all runs
    blt::size_t total_hits = 0;
    // total number of rice testing data
    blt::size_t total_tests = 0;
    // hits / tests for all runs
    blt::size_t average_hits = 0;
    blt::size_t average_tests = 0;
    // percent of valid tests
    double average_valid = 0;
    double total_fitness = 0;
    double average_fitness = 0;
};

void process_stt_file(runs_stt_data& data, int& max_gen, std::string_view file);

runs_stt_data get_per_generation_averages(const std::string& outfile, int runs);

// in case you are wondering why all these functions are using template parameters, it is so that I can pass BLT_?*_STREAM into them
// allowing for output to stdout
template<typename T>
inline void write_record(T& writer, const stt_record& r)
{
    writer << r.gen << '\t';
    writer << r.sub << '\t';
    writer << r.mean_fitness << '\t';
    writer << r.best_fitness << '\t';
    writer << r.worse_fitness << '\t';
    writer << r.mean_tree_size << '\t';
    writer << r.mean_tree_depth << '\t';
    writer << r.best_tree_size << '\t';
    writer << r.best_tree_depth << '\t';
    writer << r.worse_tree_size << '\t';
    writer << r.worse_tree_depth << '\t';
    
    writer << r.mean_fitness_run << '\t';
    writer << r.best_fitness_run << '\t';
    writer << r.worse_fitness_run << '\t';
    writer << r.mean_tree_size_run << '\t';
    writer << r.mean_tree_depth_run << '\t';
    writer << r.best_tree_size_run << '\t';
    writer << r.best_tree_depth_run << '\t';
    writer << r.worse_tree_size_run << '\t';
    writer << r.worse_tree_depth_run << '\n';
}

template<typename T, typename FUNC, typename... Args>
inline void write_data_values(T& writer, const std::string& function, const bool end, FUNC func, Args... args)
{
    const char BASE = 'a';
    for (int i = 0; i < 20; i++)
    {
        char c = static_cast<char>(BASE + i);
        writer << function;
        func(writer, c, args...);
        if (i != 19 || !end)
            writer << '\t';
        // spacing tab
        if (i == 19 && !end)
            writer << '\t';
    }
}

// writes functions operating on runs per generation
template<typename T>
inline void write_func_gens(T& writer, const char c, const int current_gen, runs_stt_data& data, const blt::size_t gen_offset, const blt::size_t offset)
{
    auto runs = data.averages[current_gen].size();
    for (size_t j = 0; j < runs; j++)
    {
        writer << c << ((gen_offset) + (offset + j));
        if (j != runs - 1)
            writer << ", ";
        else
            writer << ')';
    }
}

// operates on the aggregated data created by the above function giving totals for the entire population
template<typename T>
inline void write_func_pop(T& writer, const char c, const int gen_size, const blt::size_t offset)
{
    for (int j = 0; j < gen_size; j++)
    {
        // get the position of our aggregated data, which the offset contains
        writer << c << (offset - gen_size - 1 + j);
        if (j != gen_size - 1)
            writer << ", ";
        else
            writer << ')';
    }
}

void write_averaged_output(const std::string& writefile, const runs_stt_data& data, const std::vector<stt_record>& generation_averages);

void write_full_output(const std::string& writefile, const runs_stt_data& data, const std::vector<std::vector<stt_record>>& ordered_records);

void process_stt(const std::string& outfile, const std::string& writefile, const std::string& writefile_run, int runs);

void process_fn(const std::string& outfile, const std::string& writefile, int runs);

void process_files(const std::string& outfile, const std::string writefile, int runs);

#endif //FINALPROJECT_RUNNER_AGGREGATION_H
