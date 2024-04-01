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

#ifndef FINALPROJECT_RUNNER_RICE_LOADER_H
#define FINALPROJECT_RUNNER_RICE_LOADER_H

#include <vector>
#include <string_view>

struct rice_record
{
    double area;
    double perimeter;
    double major_axis_length;
    double minor_axis_length;
    double eccentricity;
    double convex_area;
    double extent;
    char type[8]{};
};

struct loaded_data
{
    std::vector<rice_record> cammeo;
    std::vector<rice_record> osmancik;
    std::vector<rice_record> test_set;
    std::vector<rice_record> train_set;
    // everything after these values is testing data.
    size_t last_c = 0;
    size_t last_o = 0;
    
    const std::vector<rice_record>& getTrainingSet(size_t amount);
    
    const std::vector<rice_record>& getTestingSet();
};

void vec_randomizer(std::vector<rice_record>& read, std::vector<rice_record>& write, const char* seed);

void load_rice_data(std::string_view path, loaded_data& rice_data, const char* seed);

#endif //FINALPROJECT_RUNNER_RICE_LOADER_H
