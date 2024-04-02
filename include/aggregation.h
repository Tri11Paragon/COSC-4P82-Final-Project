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

struct stt_file
{
    std::vector<stt_record> records;
    blt::size_t generations = 0;
    
    static stt_file from_file(std::string_view file);
};

/**
 * Structure used to store information loaded from the .fn file
 */
struct fn_file
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
    // hits from testing data
    blt::size_t hits = 0;
    // total data values
    blt::size_t total = 0;
    
    static fn_file from_file(std::string_view file);
};

struct run_stats
{
    stt_file stt;
    fn_file fn;
    
    static run_stats from_file(std::string_view sst_file, std::string_view fn_file);
};

struct search_stats
{
    std::vector<run_stats> runs;
};

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

void process_files(const std::string& outfile, const std::string& writefile, int runs);

#endif //FINALPROJECT_RUNNER_AGGREGATION_H
