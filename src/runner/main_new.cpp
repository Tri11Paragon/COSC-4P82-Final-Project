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
#include <random>
#include <poll.h>
#include <fcntl.h>
#include "blt/std/assert.h"
#include "blt/std/memory.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <ipc.h>

class child_t
{
    private:
        std::int32_t pid = 0;
        int socket = 0;
        bool socket_closed = false;
    public:
        
        child_t() = default;
        
        explicit child_t(blt::i32 pid): pid(pid)
        {}
        
        void open(int sock)
        {
            socket = sock;
        }
        
        ssize_t write(unsigned char* buffer, blt::size_t count)
        {
            pollfd fds_write{socket, POLLOUT, 0};
            if (poll(&fds_write, 1, 1) < 0)
                BLT_WARN("Error polling write %d", errno);
            if (fds_write.revents & POLLHUP)
                socket_closed = true;
            if (fds_write.revents & POLLERR || fds_write.revents & POLLHUP || fds_write.revents & POLLNVAL)
                return 0;
            if (fds_write.revents & POLLOUT)
                return ::write(socket, buffer, count);
            return 0;
        }
        
        ssize_t read(unsigned char* buffer, blt::size_t count)
        {
            pollfd fds_read{socket, POLLIN, 0};
            if (poll(&fds_read, 1, 1) < 0)
                BLT_WARN("Error polling read %d", errno);
            if (fds_read.revents & POLLHUP)
                socket_closed = true;
            if (fds_read.revents & POLLERR || fds_read.revents & POLLHUP || fds_read.revents & POLLNVAL)
                return 0;
            if (fds_read.revents & POLLIN)
                return ::read(socket, buffer, count);
            return 0;
        }
        
        [[nodiscard]] inline bool isSocketClosed() const
        {
            return socket_closed;
        }
        
        ~child_t()
        {
            close(socket);
        }
};

// children PIDs
blt::hashmap_t<std::int32_t, std::unique_ptr<child_t>> children;
sockaddr_un name{};
int host_socket = 0;
std::string SOCKET_LOCATION;
state_t current_state = state_t::RUN_GENERATIONS;
double fitness = 0;

int child_fp(blt::arg_parse::arg_results& args, int run_id, const std::string& socket_location)
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
    
    auto command = program + " -f " + file + " -p rice_file='" + rice_file + "' -p socket_location='" + socket_location + "' -p process_id=" +
                   std::to_string(getpid());
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

void create_child_sockets()
{
    blt::i64 ret;
    unsigned char data[sizeof(pid_t)];
    for (blt::u64 i = 0; i < children.size(); i++)
    {
        int socket_fd = accept(host_socket, nullptr, nullptr);
        BLT_ASSERT(socket_fd != -1 && "Failed to create data socket!");
        
        // wait until client sends pid data.
        do
        {
            std::memset(data, 0, sizeof(data));
            ret = read(socket_fd, data, sizeof(data));
        } while (ret != sizeof(data));
        
        pid_t pid;
        blt::mem::fromBytes(data, pid);
        //pid -= 1;
        
        if (!children.contains(pid))
            BLT_WARN("This PID '%d' does not exist as a child!", pid);
        else
            BLT_INFO("Established connection to child %d", pid);
        children[pid]->open(socket_fd);
    }
}

void remove_pending_finished_child_process()
{
    int status;
    for (const auto& pair : children)
    {
        auto pid = waitpid(pair.first, &status, WNOHANG);
        if (pid != 0 && (WIFEXITED(status) || WIFSIGNALED(status)))
        {
            BLT_TRACE("Process %d exited? %b signaled? %b", pid, WIFEXITED(status), WIFSIGNALED(status));
            auto child = std::find_if(children.begin(), children.end(), [pid](const auto& item) {
                return item.first == pid;
            });
            if (child == children.end())
            {
                BLT_WARN("Unable to find child process %d!", pid);
                return;
            }
            children.erase(child);
            
            BLT_TRACE("Closing process %d finished!", pid);
        }
    }
}

void create_parent_socket()
{
    std::memset(&name, 0, sizeof(name));
    name.sun_family = AF_UNIX;
    std::strncpy(name.sun_path, SOCKET_LOCATION.c_str(), sizeof(name.sun_path) - 1);
    
    int ret;
    
    BLT_INFO("Creating socket for %s", SOCKET_LOCATION.c_str());
    host_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    BLT_ASSERT(host_socket != -1 && "Failed to create socket!");
    ret = bind(host_socket, (const struct sockaddr*) &name, sizeof(name));
    BLT_ASSERT(ret == 0 && "Failed to bind socket");
    ret = listen(host_socket, 20);
    BLT_ASSERT(ret == 0 && "Failed to listen socket");
}

void tick_state(blt::arg_parse::arg_results& args)
{
    packet_t packet{};
    unsigned char buffer[sizeof(packet_t)];
    packet.state = current_state;
    switch (current_state)
    {
        case state_t::RUN_GENERATIONS:
        {
            auto it = children.begin();
            while (it != children.end())
            {
                packet.id = packet_id::EXECUTE_RUN;
                packet.numOfGens = args.get<blt::i32>("--num_gen");
                std::memcpy(buffer, &packet, sizeof(buffer));
                if (it->second->write(buffer, sizeof(buffer)) <= 0)
                {
                    if (it->second->isSocketClosed())
                    {
                        it = children.erase(it);
                        continue;
                    }
                    BLT_WARN("Failed to write to child error %d", errno);
                }
                ++it;
            }
            current_state = state_t::CHILD_EVALUATION;
            break;
        }
        
        case state_t::CHILD_EVALUATION:
        {
            std::vector<double> fitness_storage;
            while (fitness_storage.size() != children.size())
            {
                auto it = children.begin();
                while (it != children.end())
                {
                    auto& child = *it;
                    ssize_t ret;
                    if (ret = child.second->read(buffer, sizeof(buffer)), ret <= 0)
                    {
                        if (child.second->isSocketClosed())
                        {
                            it = children.erase(it);
                            continue;
                        }
                        if (errno != 0)
                            BLT_WARN("Failed to read to child error %d", errno);
                    } else
                    {
                        std::memcpy(&packet, buffer, sizeof(buffer));
                        BLT_ASSERT(packet.id == packet_id::CHILD_FIT && "State mismatch!");
                        fitness_storage.push_back(packet.fitness);
                    }
                    ++it;
                }
            }
            
            std::sort(fitness_storage.begin(), fitness_storage.end());
            auto ratio = args.get<double>("--prune_ratio");
            auto cutoff = static_cast<long>(static_cast<double>(fitness_storage.size()) * ratio);
            if (!fitness_storage.empty())
                fitness = fitness_storage[cutoff];
            else
                BLT_WARN("Running with no active populations?");
            BLT_INFO("Cutoff value %d, current size %d, fitness: %f", cutoff, fitness_storage.size(), fitness);
            current_state = state_t::PRUNE;
            
            break;
        }
        case state_t::PRUNE:
        {
            if (children.size() == 1)
            {
                packet.id = packet_id::PRUNE;
                packet.fitness = 0;
                std::memcpy(buffer, &packet, sizeof(buffer));
                for (auto& child : children)
                    child.second->write(buffer, sizeof(buffer));
                break;
            }
            BLT_DEBUG("Pruning with fitness %f", fitness);
            auto it = children.begin();
            while (it != children.end())
            {
                auto& child = *it;
                packet.id = packet_id::PRUNE;
                packet.fitness = fitness;
                std::memcpy(buffer, &packet, sizeof(buffer));
                if (child.second->write(buffer, sizeof(buffer)) <= 0)
                {
                    if (child.second->isSocketClosed())
                    {
                        it = children.erase(it);
                        continue;
                    }
                    BLT_WARN("Failed to write to child error %d", errno);
                }
                ++it;
            }
            current_state = state_t::RUN_GENERATIONS;
            break;
        }
    }
}

void init_sockets(blt::arg_parse::arg_results& args)
{
    create_child_sockets();
    while (!children.empty())
    {
        remove_pending_finished_child_process();
        tick_state(args);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    unlink(name.sun_path);
    close(host_socket);
}

int main(int argc, const char** argv)
{
    blt::arg_parse parser;
    
    parser.addArgument(blt::arg_builder("-n", "--num_pops").setDefault("10").setHelp("Number of populations to start").build());
    parser.addArgument(blt::arg_builder("-g", "--num_gen").setDefault("5").setHelp("Number of generations between pruning").build());
    parser.addArgument(blt::arg_builder("-p", "--prune_ratio").setDefault("0.2").setHelp("Number of generations to run before pruning").build());
    parser.addArgument(blt::arg_builder("--program").setDefault("./FinalProject").setHelp("GP Program to execute per run").build());
    parser.addArgument(blt::arg_builder("--out_file").setDefault("regress")
                                                     .setHelp("Name of the stats file (without extension) to use in building the final data")
                                                     .build());
    parser.addArgument(blt::arg_builder("--write_file").setDefault("aggregated").setHelp(
            "Name of the file to write the aggregated data to (without extension)").build());
    parser.addArgument(blt::arg_builder("--file").setDefault("../input.file").setHelp("File to run the GP on").build());
    parser.addArgument(blt::arg_builder("--rice").setDefault("../Rice_Cammeo_Osmancik.arff").setHelp("Rice file to run the GP on").build());
    
    auto args = parser.parse_args(argc, argv);
    
    BLT_INFO("%b", args.contains("--num_pops"));
    BLT_INFO(args.get<std::string>("--write_file"));
    
    BLT_INFO("Parsing user arguments:");
    for (auto& v : args.data)
        BLT_INFO("\t%s = %s", v.first.c_str(), blt::to_string(v.second).c_str());
    
    std::string random_id;
    std::random_device dev;
    std::mt19937_64 engine(dev());
    std::uniform_int_distribution charGenLower('a', 'z');
    std::uniform_int_distribution charGenUpper('A', 'Z');
    std::uniform_int_distribution choice(0, 1);
    for (int i = 0; i < 5; i++)
    {
        if (choice(engine))
            random_id += static_cast<char>(charGenLower(engine));
        else
            random_id += static_cast<char>(charGenUpper(engine));
    }
    
    auto runs = args.get<std::int32_t>("num_pops");
    BLT_DEBUG("Running with %d runs", runs);
    
    SOCKET_LOCATION = "/tmp/gp_program_" + random_id + ".socket";
    
    create_parent_socket();
    for (auto i = 0; i < runs; i++)
    {
        auto pid = fork();
        if (pid == 0)
            return child_fp(args, i, SOCKET_LOCATION);
        else if (pid > 0)
        {
            // parent
            children.insert({pid, std::make_unique<child_t>(pid)});
            BLT_TRACE("Forked child to %d", pid);
        } else
        {
            // failure
            BLT_ERROR("Failed to fork process! Error: %d", errno);
            return 1;
        }
    }
    init_sockets(args);
}
