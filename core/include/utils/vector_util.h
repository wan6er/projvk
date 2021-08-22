#pragma once

#include <vector>
#include <string>

namespace utils
{
    template<class Type>
    void filter_vector(const std::vector<Type>& comp_vec, const std::vector<Type>& target_vec, std::vector<Type>& out_vec);

    void string_to_charptr(const std::vector<std::string>& src, std::vector<const char*>& dst);
};

#include "utils/vector_util.inl"