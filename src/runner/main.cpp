/*
 *  This only works for linux!!
 *
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
#include <blt/std/logging.h>
#include <blt/parse/argparse.h>
#include <blt/profiling/profiler_v2.h>
#include <blt/std/utility.h>
#include <blt/fs/loader.h>
#include <unistd.h>
#include <sys/wait.h>
#include <thread>
#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include "blt/std/assert.h"
#include "blt/std/memory.h"

blt::hashset_t<std::int32_t> pids;

#ifndef SOURCE_DIR
    #define SOURCE_DIR ""
    #define BUILD_DIR ""
#endif

inline const std::string BASE_SOURCE_DIR = SOURCE_DIR;
inline const std::string BASE_BUILD_DIR = BUILD_DIR;

struct ofstream_wrapper
{
    private:
        std::ofstream stream_;
        blt::size_t& offset_;
    public:
        template<typename STREAM>
        ofstream_wrapper(STREAM&& stream, blt::size_t& offset): stream_(std::forward<STREAM>(stream)), offset_(offset)
        {}
        
        /**
         * Im a lazy bastard sometimes
         * Tee Hee :3
         */
        template<typename T>
        inline friend ofstream_wrapper& operator<<(ofstream_wrapper& wrapper, T&& t)
        {
            wrapper.stream_ << t;
            if constexpr (std::is_same_v<std::string, T>)
            {
                for (char c : t)
                    if (c == '\n')
                        wrapper.offset_++;
            } else if constexpr (std::is_same_v<char, T>)
            {
                if (t == '\n')
                    wrapper.offset_++;
            } else if constexpr (std::is_same_v<char*, std::remove_cv_t<T>>)
            {
                for (blt::size_t i = 0; i < std::strlen(t); i++)
                {
                    if (t[i] == '\n')
                        wrapper.offset_++;
                }
            }
            return wrapper;
        }
        
        std::ofstream& stream()
        {
            return stream_;
        }
};

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
        
        inline stt_record& operator+=(const stt_record& r)
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
        
        inline stt_record& operator/=(int i)
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
        
        static inline stt_record from_string_array(int generation, size_t& idx, blt::span<std::string> values)
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

int child(blt::arg_parse::arg_results& args, int run_id)
{
    auto program = "../" + args.get<std::string>("program");
    auto file = "../" + args.get<std::string>("file");
    auto rice_file = "../" + args.get<std::string>("rice");
    auto dir = "./run_" + std::to_string(run_id);
    BLT_DEBUG("Running GP program '%s' on run %d", program.c_str(), run_id);
    
    mkdir(dir.c_str(), S_IREAD | S_IWRITE | S_IEXEC | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH);
    if (chdir(dir.c_str()))
    {
        BLT_ERROR(errno);
        return 1;
    }
    
    auto command = "" + program + " -f " + file + " -p rice_file='" + rice_file + "'";
    BLT_TRACE("Running command %s", command.c_str());
    
    FILE* process = popen(command.c_str(), "r");
    char buffer[4096];
    while (fgets(buffer, 4096, process) != nullptr)
    {
        BLT_TRACE_STREAM << buffer;
    }
    
    pclose(process);
    
    return 0;
}

void wait_for_children()
{
    while (!pids.empty())
    {
        int status;
        auto pid = wait(&status);
        if (WIFEXITED(status))
        {
            BLT_END_INTERVAL("Program", "Process: " + std::to_string(pid));
            pids.erase(pid);
            BLT_TRACE("Process %d finished!", pid);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

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

inline runs_stt_data get_per_generation_averages(const std::string& outfile, int runs)
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

// encase you are wondering why all these functions are using template parameters, it is so that I can pass BLT_?*_STREAM into them
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

// std::function<void(std::ofstream&, const char, const Args...)>

// std::function<void(std::ofstream&, const char, const int, const int, const blt::size_t)>
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

inline void write_averaged_output(const std::string& writefile, const runs_stt_data& data, const std::vector<stt_record>& generation_averages)
{
    std::ofstream writer(writefile);
    writer << "Runs Generation Count Mean: " << data.generations_average << '\n';
    writer << "Runs Generation Count Mode: " << data.mode_generation << " occurred (" << data.generations_mode << ") time(s)\n";
    writer << "GEN#\tSUB#\tμFGEN\tFsBestGEN\tFsWorstGEN\tμTreeSzGEN\tμTreeDpGEN\tbTreeSzGEN\tbTreeDpGEN\twTreeSzGEN\twTreeDpGEN\tμFRUN\t"
              "FsBestRUN\tFsWorstRUN\tμTreeSzRUN\tμTreeDpRUN\tbTreeSzRUN\tbTreeDpRUN\twTreeSzRUN\twTreeDpRUN\n";
    
    for (const auto& r : generation_averages)
        write_record(writer, r);
}

inline void write_full_output(const std::string& writefile, const runs_stt_data& data, const std::vector<std::vector<stt_record>>& ordered_records)
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

void process_files(blt::arg_parse::arg_results& args)
{
    const auto outfile = args.get<std::string>("out_file");
    const auto writefile = args.get<std::string>("write_file");
    const auto writefile_avg = writefile + ".tsv";
    const auto writefile_run = writefile + "_runs.tsv";
    const auto writefile_fn = writefile + "_fn.tsv";
    const auto runs = args.get<std::int32_t>("runs");
    process_stt(outfile, writefile_avg, writefile_run, runs);
    if (args.contains("disable_partb") && args.get<int32_t>("disable_partb"))
        process_fn(outfile, writefile_fn, runs);
}

int main_old(int argc, const char** argv)
{
    blt::arg_parse parser;
    
    parser.addArgument(blt::arg_builder("-r", "--runs").setDefault(10).setHelp("Number of runs to execute").build());
    parser.addArgument(blt::arg_builder("-p", "--program").setDefault("./Assignment_1").setHelp("GP Program to execute per run").build());
    parser.addArgument(blt::arg_builder("-o", "--out_file").setDefault("regress")
                                                           .setHelp("Name of the stats file (without extension) to use in building the final data")
                                                           .build());
    parser.addArgument(blt::arg_builder("-w", "--write_file").setDefault("aggregated").setHelp(
            "Name of the file to write the aggregated data to (without extension)").build());
    parser.addArgument(blt::arg_builder("-f", "--file").setDefault("../input.file").setHelp("File to run the GP on").build());
    parser.addArgument(blt::arg_builder("--rice").setDefault("../Rice_Cammeo_Osmancik.arff").setHelp("Rice file to run the GP on").build());
    parser.addArgument(
            blt::arg_builder("-a", "--aggregate").setAction(blt::arg_action_t::STORE_TRUE).setHelp("Run the aggregation on existing runs folders")
                                                 .build());
    parser.addArgument(blt::arg_builder("-e", "--execute").setAction(blt::arg_action_t::STORE_TRUE).setHelp("Perform the runs").build());
    parser.addArgument(blt::arg_builder("-b", "--disable_partb").setAction(blt::arg_action_t::STORE_FALSE).setDefault(true)
                                                        .setHelp("Disable aggregation on part B data (This defaults to true)").build());
    
    auto args = parser.parse_args(argc, argv);
    
    if (!args.contains("--aggregate") && !args.contains("--execute"))
    {
        BLT_FATAL("You must select --aggregate and/or --execute");
        return 1;
    }
    
    BLT_INFO("Parsing user arguments:");
    for (auto& v : args.data)
        BLT_INFO("\t%s = %s", v.first.c_str(), blt::to_string(v.second).c_str());
    
    BLT_START_INTERVAL("Program", "Parent Total");
    if (args.contains("--execute"))
    {
        auto runs = args.get<std::int32_t>("runs");
        BLT_DEBUG("Running with %d runs", runs);
        
        for (auto i = 0; i < runs; i++)
        {
            auto pid = fork();
            if (pid == 0)
            {
                return child(args, i);
            } else if (pid > 0)
            {
                BLT_START_INTERVAL("Program", "Process: " + std::to_string(pid));
                // parent
                pids.insert(pid);
                BLT_TRACE("Forked child to %d", pid);
            } else
            {
                // failure
                BLT_ERROR("Failed to fork process! Error: %d", errno);
                return 1;
            }
        }
        wait_for_children();
    }
    if (args.contains("--aggregate"))
        process_files(args);
    BLT_END_INTERVAL("Program", "Parent Total");
    BLT_PRINT_PROFILE("Program");
    return 0;
}
