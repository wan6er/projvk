#pragma once

#include "cvk/vk_header.h"

#include <vector>
#include <string>

namespace utils
{
    
CVK_API std::vector<char> load_file(std::string CONST_REFERENCE path);

} // namespace utils
