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
#include <aggregation.h>
#include <blt/std/assert.h>
#include <blt/std/string.h>
#include <blt/fs/loader.h>
#include <algorithm>
#include <iostream>
#include <fstream>

stt_record stt_record::from_string_array(int generation, size_t& idx, blt::span<std::string> values)
{
    stt_record r{};
    std::memset(&r, 0, sizeof(stt_record));
    // I don't like the reliance on order...
    r.gen = generation;
    fill_value(r.sub, values[idx++]);
    fill_value(r.mean_fitness, values[idx++]);
    fill_value(r.best_fitness, values[idx++]);
    fill_value(r.worse_fitness, values[idx++]);
    fill_value(r.mean_tree_size, values[idx++]);
    fill_value(r.mean_tree_depth, values[idx++]);
    fill_value(r.best_tree_size, values[idx++]);
    fill_value(r.best_tree_depth, values[idx++]);
    fill_value(r.worse_tree_size, values[idx++]);
    fill_value(r.worse_tree_depth, values[idx++]);
    fill_value(r.mean_fitness_run, values[idx++]);
    fill_value(r.best_fitness_run, values[idx++]);
    fill_value(r.worse_fitness_run, values[idx++]);
    fill_value(r.mean_tree_size_run, values[idx++]);
    fill_value(r.mean_tree_depth_run, values[idx++]);
    fill_value(r.best_tree_size_run, values[idx++]);
    fill_value(r.best_tree_depth_run, values[idx++]);
    fill_value(r.worse_tree_size_run, values[idx++]);
    fill_value(r.worse_tree_depth_run, values[idx++]);
    return r;
}

stt_record& stt_record::operator+=(const stt_record& r)
{
    BLT_ASSERT_MSG(gen == r.gen && "Generation value must be equal! you have an error!",
                   (std::to_string(gen) + " vs " + std::to_string(r.gen)).c_str());
    mean_fitness += r.mean_fitness;
    best_fitness += r.best_fitness;
    worse_fitness += r.worse_fitness;
    mean_tree_size += r.mean_tree_size;
    mean_tree_depth += r.mean_tree_depth;
    
    best_tree_size += r.best_tree_size;
    best_tree_depth += r.best_tree_depth;
    worse_tree_size += r.worse_tree_size;
    worse_tree_depth += r.worse_tree_depth;
    
    mean_fitness_run += r.mean_fitness_run;
    best_fitness_run += r.best_fitness_run;
    worse_fitness_run += r.worse_fitness_run;
    mean_tree_size_run += r.mean_tree_size_run;
    mean_tree_depth_run += r.mean_tree_depth_run;
    
    best_tree_size_run += r.best_tree_size_run;
    best_tree_depth_run += r.best_tree_depth_run;
    worse_tree_size_run += r.worse_tree_size_run;
    worse_tree_depth_run += r.worse_tree_depth_run;
    
    return *this;
}

stt_record& stt_record::operator/=(int i)
{
    auto v = static_cast<double>(i);
    
    mean_fitness /= v;
    best_fitness /= v;
    worse_fitness /= v;
    mean_tree_size /= v;
    mean_tree_depth /= v;
    
    best_tree_size /= i;
    best_tree_depth /= i;
    worse_tree_size /= i;
    worse_tree_depth /= i;
    
    mean_fitness_run /= v;
    best_fitness_run /= v;
    worse_fitness_run /= v;
    mean_tree_size_run /= v;
    mean_tree_depth_run /= v;
    
    best_tree_size_run /= i;
    best_tree_depth_run /= i;
    worse_tree_size_run /= i;
    worse_tree_depth_run /= i;
    
    return *this;
}

stt_file stt_file::from_file(std::string_view file)
{
    auto lines = blt::fs::getLinesFromFile(file);
    stt_file out;
    // start from second line, first line is header
    for (std::string_view line : blt::itr_offset(lines, 1))
    {
        auto values = blt::string::split(line, '\t');
        if (values.size() <= 1)
            continue;
        else if (values.size() < 21)
        {
            BLT_WARN("Size of vector (%ld) is less than expected (21). Skipping line!", values.size());
            continue;
        }
        blt::size_t idx = 0;
        
        blt::size_t generation = std::stoi(values[idx++]);
        out.generations = std::max(out.generations, generation + 1);
        out.records.push_back(stt_record::from_string_array(static_cast<int>(generation), idx, values));
    }
    return out;
}

fn_file fn_file::from_file(std::string_view file)
{
    fn_file record;
    
    auto lines = blt::fs::getLinesFromFile(file);
    
    std::string temp = "Hits: ";
    auto hits_begin_loc = lines[0].find(temp) + temp.size();
    auto hits_end_loc = lines[0].find(',', hits_begin_loc);
    
    temp = "Total Size: ";
    auto total_begin_loc = lines[0].find(temp, hits_end_loc) + temp.size();
    auto total_end_loc = lines[0].find(',', total_begin_loc);
    
    fill_value(record.hits, lines[0].substr(hits_begin_loc, hits_end_loc - hits_begin_loc));
    fill_value(record.total, lines[0].substr(total_begin_loc, total_end_loc - total_begin_loc));
    
    // extract value from the lines
    for (auto& line : lines)
    {
        auto s = blt::string::split(line, ':');
        if (s.size() < 2)
            continue;
        line = blt::string::trim(s[1]);
    }
    
    blt::size_t idx = 1;
    fill_value(record.cc, lines[idx++]);
    fill_value(record.co, lines[idx++]);
    fill_value(record.oo, lines[idx++]);
    fill_value(record.oc, lines[idx++]);
    fill_value(record.fitness, lines[idx++]);
    
    return record;
}

run_stats run_stats::from_file(std::string_view sst_file, std::string_view fn_file)
{
    run_stats stats;
    stats.stt = stt_file::from_file(sst_file);
    stats.fn = fn_file::from_file(fn_file);
    return stats;
}


void process_stt_file(runs_stt_data& data, int& max_gen, std::string_view file)
{
    auto lines = blt::fs::getLinesFromFile(file);
    for (std::string_view line : blt::itr_offset(lines, 1))
    {
        auto values = blt::string::split(line, '\t');
        if (values.size() <= 1)
            continue;
        else if (values.size() < 21)
        {
            BLT_WARN("Size of vector (%ld) is less than expected (21). Skipping line!", values.size());
            continue;
        }
        blt::size_t idx = 0;
        
        auto generation = std::stoi(values[idx++]);
        max_gen = std::max(max_gen, generation);
        
        data.averages[generation].push_back(stt_record::from_string_array(generation, idx, values));
    }
}

void process_files(const std::string& outfile, const std::string& writefile, int runs)
{
    const auto writefile_avg = writefile + ".tsv";
    const auto writefile_run = writefile + "_runs.tsv";
    const auto writefile_fn = writefile + "_fn.tsv";
    
    search_stats stats;
    
    for (int i = 0; i < runs; i++)
    {
        std::string path = ("./run_" + (std::to_string(i) += "/")) += outfile;
        stats.runs.push_back(run_stats::from_file(path + ".stt", path + ".fn"));
    }
    
    BLT_TRACE("Hello there!");
}
