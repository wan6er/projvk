#include "utils/vector_util.h"

void utils::string_to_charptr(const std::vector<std::string>& src, std::vector<const char*>& dst)
{
    for (auto& i : src) 
    {
        dst.push_back(i.c_str());
    }
}