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
#include <rice_loader.h>
#include <random>
#include <cstring>
#include "blt/std/logging.h"
#include "lilgp.h"
#include "blt/std/types.h"
#include "blt/std/string.h"
#include "blt/std/ranges.h"
#include "blt/fs/loader.h"

const std::vector<rice_record>& loaded_data::getTrainingSet(size_t amount)
{
    if (amount == 0)
        return train_set;
    if (train_set.empty())
    {
        static std::random_device dev;
        static std::mt19937_64 engine(dev());
        std::uniform_int_distribution choice(0, 1);
        
        for (size_t i = 0; i < amount; i++)
        {
            if (last_c >= cammeo.size() || last_o >= osmancik.size())
            {
                BLT_FATAL("ERROR IN CREATING SUBSET, REQUESTED SIZE IS NOT POSSIBLE");
                std::exit(1);
            }
            if (choice(engine))
                train_set.push_back(cammeo[last_c++]);
            else
                train_set.push_back(osmancik[last_o++]);
        }
    }
    return train_set;
}

const std::vector<rice_record>& loaded_data::getTestingSet()
{
    if (test_set.empty())
    {
        static std::random_device dev;
        static std::mt19937_64 engine(dev());
        std::uniform_int_distribution choice(0, 1);
        
        while (true)
        {
            if (last_c >= cammeo.size() || last_o >= osmancik.size())
                break;
            if (choice(engine))
                test_set.push_back(cammeo[last_c++]);
            else
                test_set.push_back(osmancik[last_o++]);
        }
        
        for (; last_c < cammeo.size(); last_c++)
            test_set.push_back(cammeo[last_c]);
        for (; last_o < osmancik.size(); last_o++)
            test_set.push_back(osmancik[last_o]);
    }
    
    return test_set;
}

void vec_randomizer(std::vector<rice_record>& read, std::vector<rice_record>& write, const char* seed_param)
{
    static std::random_device dev;
    auto seed_val = dev();
    if (seed_param != nullptr)
        seed_val = std::stoi(seed_param);
    std::mt19937_64 engine(seed_val);
    while (!read.empty())
    {
        std::uniform_int_distribution choice(0ul, read.size() - 1);
        auto pos = choice(engine);
        write.push_back(read[pos]);
        std::iter_swap(read.end() - 1, read.begin() + static_cast<blt::i64>(pos));
        read.pop_back();
    }
}

void load_rice_data(std::string_view path, loaded_data& rice_data, const char* seed)
{
    auto file = blt::fs::getLinesFromFile(path);
    size_t index = 0;
    while (!blt::string::contains(file[index++], "@DATA"))
    {}
    std::vector<rice_record> c;
    std::vector<rice_record> o;
    for (std::string_view v : blt::itr_offset(file, index))
    {
        auto data = blt::string::split(v, ',');
        rice_record r{std::stod(data[0]), std::stod(data[1]), std::stod(data[2]), std::stod(data[3]), std::stod(data[4]), std::stod(data[5]),
                      std::stod(data[6])};
        std::memset(r.type, 0, 8ul);
        std::memcpy(r.type, data[7].data(), std::min(data[7].size(), 8ul));
        if (blt::string::contains(data[7], "Cammeo"))
            c.push_back(r);
        else
            o.push_back(r);
    }
    
    vec_randomizer(c, rice_data.cammeo, seed);
    vec_randomizer(o, rice_data.osmancik, seed);
}
