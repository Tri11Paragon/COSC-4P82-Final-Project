/*  lil-gp Genetic Programming System, version 1.0, 11 July 1995
 *  Copyright (C) 1995  Michigan State University
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *  
 *  Douglas Zongker       (zongker@isl.cps.msu.edu)
 *  Dr. Bill Punch        (punch@isl.cps.msu.edu)
 *
 *  Computer Science Department
 *  A-714 Wells Hall
 *  Michigan State University
 *  East Lansing, Michigan  48824
 *  USA
 *  
 */

#include <stdio.h>
#include <math.h>

#include <blt/fs/loader.h>
#include <random>
#include <cstring>
#include "blt/std/logging.h"
#include "blt/std/ranges.h"
#include "blt/std/assert.h"
#include "blt/std/memory_util.h"
#include "blt/std/error.h"
#include "blt/std/types.h"
#include "rice_loader.h"
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <thread>
#include <ipc.h>
#include <atomic>
#include <poll.h>
#include <fcntl.h>
#include <queue>
#include <mutex>

extern "C" {
#include <lilgp.h>
#include "protos.h"
}

std::unique_ptr<std::thread> network_thread;
// is the gp system still running?
std::atomic_bool running = true;
// should the system pause at the beginning of evaluation for further instructions?
std::atomic_bool paused = true;
std::atomic_bool close_requested = false;
// number of generations left to run before switching to child eval
std::atomic_int32_t generations_left = -1;
// mutex for accessing any control variable. not needed
// individual
std::atomic<double> best_individual;
// mutex for accessing the send packets queue.
std::mutex send_mutex;
std::queue<packet_t> send_packets;
int our_socket;


static int fitness_cases = -1;
#ifdef PART_B
static double* app_fitness_cases[8];
#else
static double* app_fitness_cases[2];
#endif
static double value_cutoff;

// required for this to work with c++
template<typename T>
auto cxx_d(T* t)
{
    return reinterpret_cast<double (*)()>(t);
}

template<typename T>
auto cxx_c(T* t)
{
    return reinterpret_cast<char* (*)()>(t);
}

template<typename T>
bool run_once()
{
    static bool value = true;
    return std::exchange(value, false);
}

loaded_data rice_data;

struct annoying
{
    blt::size_t cc = 0;
    blt::size_t co = 0;
    blt::size_t oo = 0;
    blt::size_t oc = 0;
};

void handle_networking()
{
    packet_t packet{};
    unsigned char buffer[sizeof(packet_t)];
    while (running)
    {
        pollfd fds_read{our_socket, POLLIN, 0};
        if (poll(&fds_read, 1, 1) < 0)
            BLT_WARN("Error polling read %d", errno);
        if (fds_read.revents & POLLIN)
        {
            auto bytes_read = read(our_socket, buffer, sizeof(buffer));
            if (bytes_read > 0)
            {
                std::memcpy(&packet, buffer, sizeof(buffer));
                switch (packet.id)
                {
                    case packet_id::EXECUTE_RUN:
                        generations_left = packet.numOfGens;
                        paused = false;
                        BLT_DEBUG("Beginning execution of %d runs", generations_left.load());
                        break;
                    case packet_id::CHILD_FIT:
                        break;
                    case packet_id::PRUNE:
                    {
                        BLT_DEBUG("We are a child who is going to be killed!");
                        close(our_socket);
                        //std::exit(0);
                        close_requested = true;
                        running = false;
                        break;
                    }
                        break;
                    case packet_id::AVG_FIT:
                    case packet_id::AVG_TREE:
                    case packet_id::BEST_FIT:
                        BLT_ERROR("This should not have been sent to the client!");
                        break;
                }
            }
        }
        pollfd fds_write{our_socket, POLLOUT, 0};
        if (poll(&fds_write, 1, 1) < 0)
            BLT_WARN("Error polling write %d", errno);
        if (fds_write.revents & POLLOUT)
        {
            {
                std::scoped_lock lock(send_mutex);
                if (send_packets.empty())
                    continue;
                packet = send_packets.front();
                send_packets.pop();
            }
            BLT_INFO("Sending packet of id %d", static_cast<int>(packet.id));
            blt::logging::flush();
            std::memcpy(buffer, &packet, sizeof(buffer));
            if (write(our_socket, buffer, sizeof(buffer)) <= 0)
                BLT_WARN("No bytes written");
            blt::logging::flush();
        }
    };
}

extern "C" int app_begin_of_evaluation(int gen, multipop* mpop)
{
    BLT_INFO("Running begin of eval, current state: are we paused? %s num of gens left %d", paused ? "true" : "false", generations_left.load());
    if (generations_left == 0)
    {
        BLT_DEBUG("There are no more generations left!");
        // no more generations to run!
        paused = true;
        // inform server
        packet_t packet{};
        packet.id = packet_id::CHILD_FIT;
        packet.fitness = best_individual.load();
        {
            std::scoped_lock lock(send_mutex);
            send_packets.push(packet);
        }
        BLT_DEBUG("Beginning await next batch start");
    }
    blt::logging::flush();
    // i love busy looping
    while (paused)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    return close_requested;
}

extern "C" int app_end_of_evaluation(int gen, multipop* mpop, int newbest, popstats* gen_stats, popstats* run_stats)
{
    generations_left--;
    set_current_individual(gen_stats[0].best[0]->ind);
    best_individual.store(gen_stats[0].best[0]->ind->a_fitness);

    // {
    //     double fitness = 0;
    //     for (int i = 0; i < mpop[0].size; i++){
    //         fitness += mpop[0].pop[i]->ind->a_fitness;
    //     }

    //     packet_t packet;
    //     packet.id = packet_id::AVG_FIT;
    //     std::scoped_lock lock(send_mutex);
    //     send_packets.push(packet);
    // }
    
    if (newbest)
    {
        output_stream_open(OUT_USER);
        
        auto ind = run_stats[0].best[0]->ind;

#ifdef PART_B
        //oprintf(OUT_USER, 20, "(%lf, %lf)\n", g->x, v);
        globaldata* g = get_globaldata();
        if (g->current_individual != ind)
        {
            BLT_WARN("Huh!?!?!?!?!?");
        }
        set_current_individual(ind);
        best_individual.store(gen_stats[0].best[0]->ind->a_fitness);
        
        auto testing = rice_data.getTestingSet();
        
        annoying results;
        
        for (const auto& r : testing)
        {
            g->area = r.area;
            g->perimeter = r.perimeter;
            g->major_axis_length = r.major_axis_length;
            g->minor_axis_length = r.minor_axis_length;
            g->eccentricity = r.eccentricity;
            g->convex_area = r.convex_area;
            g->extent = r.extent;
            auto dv = r.type[0] == 'C';
            auto v = evaluate_tree(ind->tr[0].data, 0);
            
            // (real value) (predicted value)
            if (dv)
            {
                if (v >= 0)
                    results.cc++; // cammeo cammeo
                else if (v < 0)
                    results.co++; // cammeo osmancik
            } else
            {
                if (v < 0)
                    results.oo++; // osmancik osmancik
                else if (v >= 0)
                    results.oc++; // osmancik cammeo
            }
        }
        
        oprintf(OUT_USER, 50, "Hits: %ld, Total Size: %ld, Percent Hit: %lf\n", results.cc + results.oo, testing.size(),
                static_cast<double>(results.cc + results.oo) / static_cast<double>(testing.size()) * 100);
        oprintf(OUT_USER, 50, "CC: %ld\nCO: %ld\nOO: %ld\nOC: %ld\n", results.cc, results.co, results.oo, results.oc);
        oprintf(OUT_USER, 50, "Fitness: %lf\n", ind->a_fitness);
        oprintf(OUT_USER, 50, "Hits: %d\n", ind->hits);
        oprintf(OUT_USER, 50, "\n");
#endif
        pretty_print_tree_equ(ind->tr[0].data, output_filehandle(OUT_USER));
        pretty_print_tree(ind->tr[0].data, output_filehandle(OUT_USER));
        
        output_stream_close(OUT_USER);
        
        if (run_stats[0].best[0]->ind->hits == fitness_cases)
            return 1;
    }
    
    return 0;
}

extern "C" int app_initialize(int startfromcheckpoint)
{
    network_thread = std::make_unique<std::thread>(handle_networking);
    BLT_INFO("Init app");
    // TODO: maybe make this dynamic
#ifdef PART_B
    // evil hack
    auto loc_param = get_parameter("rice_file");
    auto loc_socket = get_parameter("socket_location");
    auto loc_pid = get_parameter("process_id");
    BLT_ASSERT(loc_param != nullptr && "You must provide a rice file to operate on!");
    BLT_ASSERT(loc_socket != nullptr && "You must provide a location to a unix socket!");
    BLT_ASSERT(loc_pid != nullptr && "You must provide a pid!");
    pid_t pid = std::stoi(std::string(loc_pid));
    
    BLT_INFO("Begin socket init %s", loc_socket);
    
    our_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    
    BLT_INFO("Socket Created! %d", our_socket);
    
    if (our_socket == -1)
    {
        BLT_FATAL("Failed to create sockets!");
        std::exit(4);
    }
    
    sockaddr_un our_name{};
    std::memset(&our_name, 0, sizeof(our_name));
    our_name.sun_family = AF_UNIX;
    std::strncpy(our_name.sun_path, loc_socket, sizeof(our_name.sun_path) - 1);
    
    BLT_INFO("Attempting to connect to socket %s", loc_socket);
    blt::logging::flush();
    int ret;
    do
    {
        ret = connect(our_socket, (const struct sockaddr*) &our_name, sizeof(our_name));
        if (run_once<struct socket_connect>() && ret != 0)
        {
            BLT_WARN("Unable to connect to socket.");
            BLT_WARN("System will wait until socket becomes available.");
            BLT_WARN("Please ensure this software was ran with the associated launcher!");
            BLT_WARN(errno);
            blt::error::print_socket_error();
            blt::logging::flush();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    } while (ret != 0);
    BLT_ASSERT(ret == 0 && "Failed to connect to socket");
    BLT_INFO("Connected to socket %s", loc_socket);
    blt::logging::flush();
    
    if (fcntl(our_socket, F_SETFD, fcntl(our_socket, F_GETFD) | O_NONBLOCK))
        BLT_WARN("Unable to change socket file descriptor flags; error '%d'", errno);
    
    unsigned char pid_buffer[sizeof(pid_t)];
    std::memset(pid_buffer, 0, sizeof(pid_buffer));
    blt::mem::toBytes(pid, pid_buffer);
    
    write(our_socket, pid_buffer, sizeof(pid_buffer));
    
    load_rice_data(loc_param, rice_data, get_parameter("random_seed"));
#endif
    int i;
    double x, y;
    char* param;
    globaldata* g = get_globaldata();
    
    if (!startfromcheckpoint)
    {
        oprintf(OUT_PRG, 50, "not starting from checkpoint file.\n");
        
        param = get_parameter("app.fitness_cases");
        if (param == NULL)
        {
#ifdef PART_B
            // about 30% of the data
            fitness_cases = 1000;
#else
            fitness_cases = 200;
#endif
        } else
        {
            fitness_cases = atoi(param);
            if (fitness_cases < 0)
                error(E_FATAL_ERROR, "invalid value for \"app.fitness_cases\".");
        }

#ifdef PART_B
        app_fitness_cases[0] = (double*) MALLOC(fitness_cases * sizeof(double));
        app_fitness_cases[1] = (double*) MALLOC(fitness_cases * sizeof(double));
        app_fitness_cases[2] = (double*) MALLOC(fitness_cases * sizeof(double));
        app_fitness_cases[3] = (double*) MALLOC(fitness_cases * sizeof(double));
        app_fitness_cases[4] = (double*) MALLOC(fitness_cases * sizeof(double));
        app_fitness_cases[5] = (double*) MALLOC(fitness_cases * sizeof(double));
        app_fitness_cases[6] = (double*) MALLOC(fitness_cases * sizeof(double));
        app_fitness_cases[7] = (double*) MALLOC(fitness_cases * sizeof(double));
        auto data = rice_data.getTrainingSet(fitness_cases);
#else
        app_fitness_cases[0] = (double*) MALLOC(fitness_cases * sizeof(double));
        app_fitness_cases[1] = (double*) MALLOC(fitness_cases * sizeof(double));
#endif
        
        oprintf(OUT_PRG, 50, "%d fitness cases:\n", fitness_cases);
        for (i = 0; i < fitness_cases; ++i)
        {
#ifndef PART_B
            const double range = 10;
            const double half_range = range / 2.0;
            x = (random_double(&globrand) * range) - half_range;
            
            /* change this line to modify the goal function. */
            y = x * x * x * x + x * x * x + x * x + x;
////            y = x * cos(x) + sin(x) * log(x + x * x + x) + x * x;
///*			y = x*x; */
            
            app_fitness_cases[0][i] = x;
            app_fitness_cases[1][i] = y;
            oprintf(OUT_PRG, 50, "    x = %12.5lf, y = %12.5lf\n", x, y);
#else
            app_fitness_cases[0][i] = data[i].area;
            app_fitness_cases[1][i] = data[i].perimeter;
            app_fitness_cases[2][i] = data[i].major_axis_length;
            app_fitness_cases[3][i] = data[i].minor_axis_length;
            app_fitness_cases[4][i] = data[i].eccentricity;
            app_fitness_cases[5][i] = data[i].convex_area;
            app_fitness_cases[6][i] = data[i].extent;
            app_fitness_cases[7][i] = (data[i].type[0] == 'C') ? 50 : -50;
            oprintf(OUT_PRG, 50,
                    "    area = %12.5lf, perimeter = %12.5lf, major_axis_length = %12.5lf, minor_axis_length = %12.5lf, eccentricity = %12.5lf, convex_area = %12.5lf, extent = %12.5lf, type = %c\n",
                    data[i].area, data[i].perimeter, data[i].major_axis_length, data[i].minor_axis_length, data[i].eccentricity, data[i].convex_area,
                    data[i].extent, data[i].type[0]);
#endif
        
        }
    } else
    {
        oprintf(OUT_PRG, 50, "started from checkpoint file.\n");
    }
    
    param = get_parameter("app.value_cutoff");
    if (param == NULL)
        value_cutoff = 1.e15;
    else
        value_cutoff = strtod(param, NULL);
    
    return 0;
}

extern "C" void app_uninitialize(void)
{
    running = false;
    if (network_thread->joinable())
        network_thread->join();
    network_thread = nullptr;
    close(our_socket);
    FREE(app_fitness_cases[0]);
    FREE(app_fitness_cases[1]);
}

extern "C" void app_end_of_breeding(int gen, multipop* mpop)
{}

extern "C" int app_create_output_streams(void)
{
    if (create_output_stream(OUT_USER, ".fn", 1, "w", 0) != OUTPUT_OK)
        return 1;
    return 0;
}

extern "C" int app_build_function_sets(void)
{
    function_set fset;
    user_treeinfo tree_map;
    function sets[] =
            {{cxx_d(f_multiply), nullptr, nullptr, 2, "*", FUNC_DATA, -1, 0, 0, {0, 0}},
             {cxx_d(f_protdivide), nullptr, nullptr, 2, "/", FUNC_DATA, -1, 0, 0, {0, 0}},
             {cxx_d(f_add), nullptr, nullptr, 2, "+", FUNC_DATA, -1, 0, 0, {0, 0}},
             {cxx_d(f_subtract), nullptr, nullptr, 2, "-", FUNC_DATA, -1, 0, 0, {0, 0}},
             {cxx_d(f_exp), nullptr, nullptr, 1, "exp", FUNC_DATA, -1, 0, 0, {0, 0}},
             {cxx_d(f_rlog), nullptr, nullptr, 1, "log", FUNC_DATA, -1, 0, 0, {0, 0}},
#ifndef PART_B
                    {cxx_d(f_sin), nullptr, nullptr, 1, "sin", FUNC_DATA, -1, 0, 0, {0, 0}},
                    {cxx_d(f_cos), nullptr, nullptr, 1, "cos", FUNC_DATA, -1, 0, 0, {0, 0}},
                    {cxx_d(f_var), nullptr, nullptr, 0, "x", TERM_NORM, -1, 0, 0, {0, 0}},
#else
             {cxx_d(f_area), nullptr, nullptr, 0, "area", TERM_NORM, -1, 0, 0, {0, 0}},
             {cxx_d(f_perimeter), nullptr, nullptr, 0, "perimeter", TERM_NORM, -1, 0, 0, {0, 0}},
             {cxx_d(f_major_axis_length), nullptr, nullptr, 0, "major", TERM_NORM, -1, 0, 0, {0, 0}},
             {cxx_d(f_minor_axis_length), nullptr, nullptr, 0, "minor", TERM_NORM, -1, 0, 0, {0, 0}},
             {cxx_d(f_eccentricity), nullptr, nullptr, 0, "eccentricity", TERM_NORM, -1, 0, 0, {0, 0}},
             {cxx_d(f_convex_area), nullptr, nullptr, 0, "convex", TERM_NORM, -1, 0, 0, {0, 0}},
             {cxx_d(f_extent), nullptr, nullptr, 0, "extent", TERM_NORM, -1, 0, 0, {0, 0}},
#endif
             {nullptr, f_erc_gen, cxx_c(f_erc_print), 0, "R", TERM_ERC, -1, 0, 0, {0, 0}}};
    
    binary_parameter("app.use_ercs", 1);
#ifndef PART_B
    if (atoi(get_parameter("app.use_ercs")))
        fset.size = 10;
    else
        fset.size = 9;
#else
    if (atoi(get_parameter("app.use_ercs")))
        fset.size = 14;
    else
        fset.size = 13;
#endif
    
    fset.cset = sets;
    
    tree_map.fset = 0;
    tree_map.return_type = 0;
    tree_map.name = "TREE";
    
    return function_sets_init(&fset, 1, &tree_map, 1);
}

extern "C" void app_eval_fitness(individual* ind)
{
#ifdef PART_B
    int i;
    double v;
    bool dv;
#else
    int i;
    double v, dv;
    double disp;
#endif
    globaldata* g = get_globaldata();
    
    set_current_individual(ind);
    
    ind->r_fitness = 0.0;
    ind->hits = 0;
    
    for (i = 0; i < fitness_cases; ++i)
    {
#ifdef PART_B
        g->area = app_fitness_cases[0][i];
        g->perimeter = app_fitness_cases[1][i];
        g->major_axis_length = app_fitness_cases[2][i];
        g->minor_axis_length = app_fitness_cases[3][i];
        g->eccentricity = app_fitness_cases[4][i];
        g->convex_area = app_fitness_cases[5][i];
        g->extent = app_fitness_cases[6][i];
        dv = app_fitness_cases[7][i] > 0;
#else
        g->x = app_fitness_cases[0][i];
        dv = app_fitness_cases[1][i];
#endif
        v = evaluate_tree(ind->tr[0].data, 0);
#ifdef PART_B
        if ((v >= 0 && dv) || (v < 0 && !dv))
            ind->hits++;
        ind->r_fitness = ind->hits;
        ind->s_fitness = ind->r_fitness;
        ind->a_fitness = 1 - (1 / (1 + ind->s_fitness));
#else
        disp = fabs(dv - v);
        
        if (disp < value_cutoff)
        {
            ind->r_fitness += disp;
            if (disp <= 0.01)
                ++ind->hits;
        } else
        {
            ind->r_fitness += value_cutoff;
        }
        ind->s_fitness = ind->r_fitness;
        ind->a_fitness = 1 / (1 + ind->s_fitness);
#endif
    }
    
    ind->evald = EVAL_CACHE_VALID;
}

extern "C" void app_write_checkpoint(FILE* f)
{
    int i;
    fprintf(f, "fitness-cases: %d\n", fitness_cases);
    for (i = 0; i < fitness_cases; ++i)
    {
        write_hex_block(app_fitness_cases[0] + i, sizeof(double), f);
        fputc(' ', f);
        write_hex_block(app_fitness_cases[1] + i, sizeof(double), f);
        fprintf(f, " %.5lf %.5lf\n",
                app_fitness_cases[0][i], app_fitness_cases[1][i]);
    }
}

extern "C" void app_read_checkpoint(FILE* f)
{
    int i;
    
    fscanf(f, "%*s %d\n", &fitness_cases);
    
    app_fitness_cases[0] = (double*) MALLOC(fitness_cases *
                                            sizeof(double));
    app_fitness_cases[1] = (double*) MALLOC(fitness_cases *
                                            sizeof(double));
    
    for (i = 0; i < fitness_cases; ++i)
    {
        read_hex_block(app_fitness_cases[0] + i, sizeof(double), f);
        fgetc(f);
        read_hex_block(app_fitness_cases[1] + i, sizeof(double), f);
        fscanf(f, " %*f %*f\n");
        fprintf(stderr, "%.5lf %.5lf\n", app_fitness_cases[0][i],
                app_fitness_cases[1][i]);
    }
}
