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

fn_file& fn_file::operator+=(const fn_file& r)
{
    cc += r.cc;
    co += r.co;
    oc += r.oc;
    oo += r.oo;
    fitness += r.fitness;
    hits += r.hits;
    total += r.total;
    return *this;
}

fn_file& fn_file::operator/=(int i)
{
    cc /= i;
    co /= i;
    oo /= i;
    oc /= i;
    fitness /= i;
    hits /= i;
    total /= i;
    return *this;
}

run_stats run_stats::from_file(std::string_view sst_file, std::string_view fn_file, const process_info_t& pi)
{
    run_stats stats;
    stats.stt = stt_file::from_file(sst_file);
    stats.fn = fn_file::from_file(fn_file);
    stats.process_info = pi;
    return stats;
}

std::vector<run_stats> full_stats::getBestFromGenerations()
{
    std::sort(runs.begin(), runs.end(), [](const run_stats& a, const run_stats& b) {
        return a.stt.generations > b.stt.generations;
    });
    std::vector<run_stats> best;
    blt::size_t generation = runs[0].stt.generations;
    for (const auto& v : runs)
    {
        if (v.stt.generations == generation)
            best.push_back(v);
    }
    return best;
}

std::vector<run_stats> full_stats::getBestFromFitness()
{
    std::sort(runs.begin(), runs.end(), [](const run_stats& a, const run_stats& b) {
        return a.fn.fitness > b.fn.fitness;
    });
    std::vector<run_stats> best;
    double fitness = runs[0].fn.fitness;
    for (const auto& v : runs)
    {
        if (v.fn.fitness >= fitness)
            best.push_back(v);
    }
    return best;
}

std::vector<run_stats> full_stats::getBestFromHits()
{
    std::sort(runs.begin(), runs.end(), [](const run_stats& a, const run_stats& b) {
        return (static_cast<double>(a.fn.hits) / static_cast<double>(a.fn.total)) >
               (static_cast<double>(b.fn.hits) / static_cast<double>(b.fn.total));
    });
    std::vector<run_stats> best;
    double hits = (static_cast<double>(runs[0].fn.hits) / static_cast<double>(runs[0].fn.total));
    for (const auto& v : runs)
    {
        if ((static_cast<double>(v.fn.hits) / static_cast<double>(v.fn.total)) >= hits)
            best.push_back(v);
    }
    return best;
}

averaged_stats averaged_stats::from_vec(const std::vector<run_stats>& runs)
{
    run_stats stats;
    
    auto it = runs.begin();
    
    // populate stt stats
    for (const auto& v : it->stt.records)
        stats.stt.records.push_back(v);
    // populate fn file
    stats.fn = it->fn;
    // populate process info
    stats.process_info = it->process_info;
    
    while (++it != runs.end())
    {
        // combine stt records
        for (const auto& v : blt::enumerate(it->stt.records))
            stats.stt.records[v.first] += v.second;
        // combine fn records
        stats.fn += it->fn;
        // combine process info records
        stats.process_info += it->process_info;
    }
    
    // take the mean
    stats.fn /= static_cast<int>(runs.size());
    stats.process_info /= static_cast<int>(runs.size());
    for (auto& v : stats.stt.records)
        v /= static_cast<int>(runs.size());
    
    return averaged_stats(stats, runs.size());
}

void process_files(const std::string& outfile, const std::string& writefile, int runs, blt::hashmap_t<blt::i32, process_info_t>& run_processes)
{
    full_stats stats;
    
    for (int i = 0; i < runs; i++)
    {
        std::string path = ("./run_" + (std::to_string(i) += "/")) += outfile;
        stats.runs.push_back(run_stats::from_file(path + ".stt", path + ".fn", run_processes[i]));
    }
    
    auto best_gen = stats.getBestFromGenerations();
    
//    for (const auto& v : best_gen)
//    {
//        write_fn_file(std::cout, v.fn);
//        for (const auto& r : v.stt.records)
//        {
//            write_stt_record(std::cout, r);
//        }
//        std::cout << "\n\n";
//    }
    
    auto best_gens = averaged_stats::from_vec(best_gen);
    auto best_fit = averaged_stats::from_vec(stats.getBestFromFitness());
    auto best_hits = averaged_stats::from_vec(stats.getBestFromHits());
    auto best_all = averaged_stats::from_vec(stats.runs);
    
    std::ofstream writer_best_gens(writefile + "_best_generations.tsv");
    std::ofstream writer_best_fit(writefile + "_best_fitness.tsv");
    std::ofstream writer_best_hits(writefile + "_best_hits.tsv");
    std::ofstream writer_best_all(writefile + "_all.tsv");
    
    writer_best_gens << "Aggregation Count:\t" << best_gens.count << '\n';
    writer_best_fit << "Aggregation Count:\t" << best_fit.count << '\n';
    writer_best_hits << "Aggregation Count:\t" << best_hits.count << '\n';
    writer_best_all << "Aggregation Count:\t" << best_all.count << '\n';
    
    write_process_info(writer_best_gens, best_gens.stats.process_info);
    write_process_info(writer_best_fit, best_fit.stats.process_info);
    write_process_info(writer_best_hits, best_hits.stats.process_info);
    write_process_info(writer_best_all, best_all.stats.process_info);
    
    write_fn_file(writer_best_gens, best_gens.stats.fn);
    write_fn_file(writer_best_fit, best_fit.stats.fn);
    write_fn_file(writer_best_hits, best_hits.stats.fn);
    write_fn_file(writer_best_all, best_all.stats.fn);
    
    write_stt_header(writer_best_gens);
    write_stt_header(writer_best_fit);
    write_stt_header(writer_best_hits);
    write_stt_header(writer_best_all);
    
    for (const auto& v : best_gens.stats.stt.records)
        write_stt_record(writer_best_gens, v);
    for (const auto& v : best_fit.stats.stt.records)
        write_stt_record(writer_best_fit, v);
    for (const auto& v : best_hits.stats.stt.records)
        write_stt_record(writer_best_hits, v);
    for (const auto& v : best_all.stats.stt.records)
        write_stt_record(writer_best_all, v);
}