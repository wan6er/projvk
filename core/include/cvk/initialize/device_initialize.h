#pragma once

#include "cvk/vk_header.h"

#include <vector>

namespace __cvk
{
    void get_all_device(VkInstance instance, std::vector<VkPhysicalDevice>& devices);
};