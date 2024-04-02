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

runs_stt_data get_per_generation_averages(const std::string& outfile, int runs)
{
    runs_stt_data data;
    
    for (int i = 0; i < runs; i++)
    {
        int max_gen = 0;
        auto file = "./run_" + ((std::to_string(i) += "/") += outfile) += ".stt";
        process_stt_file(data, max_gen, file);
        data.largest_generation = std::max(data.largest_generation, max_gen);
        data.total_generations += max_gen;
        data.runs_generation_size.push_back(max_gen);
    }
    
    // zero out the buffer
    // I am using my scoped buffer because it is meant to be a std::array<T, S> but at runtime.
    // it provides iterators and automatic resource management :3
    //data.generations_size_count = blt::scoped_buffer<int>(static_cast<blt::size_t>(data.largest_generation) + 1);
    data.generations_size_count.resize(data.largest_generation + 1);
    std::memset(data.generations_size_count.data(), 0, data.generations_size_count.size() * sizeof(int));
    
    // count the number of generations per length
    for (auto v : data.runs_generation_size)
        data.generations_size_count[v]++;
    
    // find the larger length that contains the most runs
    for (auto v : blt::enumerate(data.generations_size_count))
    {
        if (v.second > data.generations_size_count[data.mode_generation])
            data.mode_generation = static_cast<int>(v.first);
    }
    data.generations_mode = data.generations_size_count[data.mode_generation];
    data.generations_average = data.total_generations / runs;
    
    return data;
}

void write_averaged_output(const std::string& writefile, const runs_stt_data& data, const std::vector<stt_record>& generation_averages)
{
    std::ofstream writer(writefile);
    writer << "Runs Generation Count Mean: " << data.generations_average << '\n';
    writer << "Runs Generation Count Mode: " << data.mode_generation << " occurred (" << data.generations_mode << ") time(s)\n";
    writer << "GEN#\tSUB#\tμFGEN\tFsBestGEN\tFsWorstGEN\tμTreeSzGEN\tμTreeDpGEN\tbTreeSzGEN\tbTreeDpGEN\twTreeSzGEN\twTreeDpGEN\tμFRUN\t"
              "FsBestRUN\tFsWorstRUN\tμTreeSzRUN\tμTreeDpRUN\tbTreeSzRUN\tbTreeDpRUN\twTreeSzRUN\twTreeDpRUN\n";
    
    for (const auto& r : generation_averages)
        write_record(writer, r);
}

void write_full_output(const std::string& writefile, const runs_stt_data& data, const std::vector<std::vector<stt_record>>& ordered_records)
{
    // 1 (our ints start at zero, calc starts at 1) + 2 (two offset rows, (name row, blank row)) + written_gen_count + 1 (include gen 0)
    blt::size_t offset = 3 + data.largest_generation + 1;
    std::ofstream writer(writefile);
    // number of times we are going to create aggregated rows which need headers
    const int header_count = 3;
    for (int i = 0; i < header_count; i++)
    {
        writer << "GEN#\tSUB#\tμFGEN\tFsBestGEN\tFsWorstGEN\tμTreeSzGEN\tμTreeDpGEN\tbTreeSzGEN\tbTreeDpGEN\twTreeSzGEN\twTreeDpGEN\tμFRUN\t"
                  "FsBestRUN\tFsWorstRUN\tμTreeSzRUN\tμTreeDpRUN\tbTreeSzRUN\tbTreeDpRUN\twTreeSzRUN\twTreeDpRUN";
        if (i == header_count - 1)
            writer << '\n';
        else
            writer << "\t\t";
    }
    
    const std::string AVG_FN = "=AVERAGE(";
    const std::string STDEV_FN = "=STDEV.P(";
    
    // not every run will end at the same generation, this accounts for that by keeping a running total
    blt::size_t gen_offset = 0;
    
    // per generation, we need to write the aggregated data values using our funny functions
    for (int gen = 0; gen < data.largest_generation + 1; gen++)
    {
        // I spent way too long trying to figure out how to make c++ do the function template deduction, might be only possible in c++20 without hacks
        // this is the best solution without wasting the rest of the day.
        write_data_values(writer, AVG_FN, false, write_func_gens<std::ofstream>, gen, data, gen_offset, offset);
        write_data_values(writer, STDEV_FN, gen != 0, write_func_gens<std::ofstream>, gen, data, gen_offset, offset);
        if (gen == 0)
            write_data_values(writer, STDEV_FN, true, write_func_pop<std::ofstream>, data.largest_generation + 1, offset);
        writer << '\n';
        gen_offset += data.averages.at(gen).size();
    }
    writer << '\n';
    
    // after we create the rows which will calculate the required data for us, we can write all the runs information into the file. Ordered by gen
    for (const auto& run : ordered_records)
        for (const auto& v : run)
            write_record(writer, v);
}

void process_stt(const std::string& outfile, const std::string& writefile, const std::string& writefile_run, int runs)
{
    BLT_INFO("Processing .sst file");
    BLT_DEBUG("Loading generation data");
    auto data = get_per_generation_averages(outfile, runs);
    
    BLT_DEBUG("Ordering records and creating averages");
    // the maps are not stored in an ordered format, we will aggregate the data into usable sets
    std::vector<std::vector<stt_record>> ordered_records;
    std::vector<stt_record> generation_averages;
    for (const auto& a : data.averages)
    {
        ordered_records.push_back(a.second);
        stt_record base{};
        std::memset(&base, 0, sizeof(stt_record));
        base.gen = a.first;
        for (const auto& r : a.second)
            base += r;
        base /= runs;
        generation_averages.push_back(base);
    }
    
    BLT_DEBUG("Sorting");
    // which are then sorted
    std::sort(generation_averages.begin(), generation_averages.end(), [](const auto& a, const auto& b) {
        return a.gen < b.gen;
    });
    
    std::sort(ordered_records.begin(), ordered_records.end(), [](const auto& a, const auto& b) {
        BLT_ASSERT(!(a.empty() || b.empty()));
        return a[0].gen < b[0].gen;
    });
    
    // and written to the aggregated files
    BLT_DEBUG("Writing to average output");
    write_averaged_output(writefile, data, generation_averages);
    BLT_DEBUG("Writing full stt_record output");
    write_full_output(writefile_run, data, ordered_records);
    
    BLT_INFO("Average Number of Generations: %d", data.generations_average);
    BLT_INFO("Processing .stt file complete!");
}

void process_fn(const std::string& outfile, const std::string& writefile, int runs)
{
    BLT_INFO("Processing .fn file");
    
    runs_fn_data data;
    
    blt::size_t best_hits = 0;
    for (int i = 0; i < runs; i++)
    {
        auto file = "./run_" + ((std::to_string(i) += "/") += outfile) += ".fn";
        auto lines = blt::fs::getLinesFromFile(file);
        
        // extract value from the lines
        for (auto& line : lines)
        {
            auto s = blt::string::split(line, ':');
            if (s.size() < 2)
                continue;
            line = blt::string::trim(s[1]);
        }
        
        blt::size_t idx = 1;
        fn_record record;
        fill_value(record.cc, lines[idx++]);
        fill_value(record.co, lines[idx++]);
        fill_value(record.oo, lines[idx++]);
        fill_value(record.oc, lines[idx++]);
        fill_value(record.fitness, lines[idx++]);
        fill_value(record.hits, lines[idx++]);
        if (record.hits > best_hits)
        {
            best_hits = record.cc + record.oo;
            data.best = data.runs.size();
        }
        data.runs.push_back(record);
    }
    
    std::ofstream writer(writefile);
    writer << "Run(RV)/(PV)\tCC\tCO\tOO\tOC\n";
    
    blt::size_t cc = 0, co = 0, oo =0, oc = 0;
    for (auto e : blt::enumerate(data.runs))
    {
        const auto& v = e.second;
        data.total_hits += v.oo + v.cc;
        data.total_tests += v.oo + v.cc + v.co + v.oc;
        cc += v.cc;
        co += v.co;
        oo += v.oo;
        oc += v.oc;
        writer << e.first << '\t' << v.cc << '\t' << v.co << '\t' << v.oo << '\t' << v.oc << '\n';
    }
    
    data.average_hits = data.total_hits / runs;
    data.average_tests = data.total_tests / runs;
    data.average_valid = static_cast<double>(data.average_hits) / static_cast<double>(data.average_tests);
    
    writer << "\nBest Result:\n";
    auto best = data.runs[data.best];
    writer << "\tCC\tCO\tOO\tOC\n";
    writer << '\t' << best.cc << '\t' << best.co << '\t' << best.oo << '\t' << best.oc << '\n';
    writer << "Fitness:\t" << best.fitness;
    
    writer << "\nTotal results:\n";
    writer << "\tCC\tCO\tOO\tOC\n";
    writer << '\t' << cc << '\t' << co << '\t' << oo << '\t' << oc << '\n';
    
    writer << "\nAveraged results:\n";
    writer << "\tCC\tCO\tOO\tOC\n";
    writer << '\t' << cc / runs << '\t' << co / runs << '\t' << oo / runs << '\t' << oc / runs << '\n';
    
    writer << "\n\n";
    writer << "Average Hits Per Run:\t" << data.average_hits << '\n';
    writer << "Average Tests Per Run:\t" << data.average_tests << '\n';
    writer << "Average % Correct Per Run:\t" << data.average_valid << '\n';
    writer << "Total Hits(All runs combined):\t" << data.total_hits << '\n';
    writer << "Total Tests(All runs combined):\t" << data.total_tests << '\n';
    
    BLT_INFO("Processing .fn file complete!");
}

void process_files(const std::string& outfile, const std::string writefile, int runs)
{
    const auto writefile_avg = writefile + ".tsv";
    const auto writefile_run = writefile + "_runs.tsv";
    const auto writefile_fn = writefile + "_fn.tsv";
    process_stt(outfile, writefile_avg, writefile_run, runs);
    process_fn(outfile, writefile_fn, runs);
}