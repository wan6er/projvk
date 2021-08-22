#pragma once

#include <algorithm>

#include "clog.h"

template <class Type>
void utils::filter_vector(const std::vector<Type> &comp_vec, const std::vector<Type> &target_vec, std::vector<Type> &out_vec)
{

    for (auto item : target_vec)
    {
        if (std::find(comp_vec.begin(), comp_vec.end(), item) == comp_vec.end()) 
        {
            CLogE("Enabled instance extension %s failed.", item);
        }
        else 
        {
            out_vec.push_back(item);
        }
    }
}