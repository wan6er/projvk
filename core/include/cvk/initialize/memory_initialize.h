#pragma once

#include "cvk/vk_header.h"

namespace __cvk
{
    VkResult create_memory(const VkPhysicalDeviceMemoryProperties& all_properties, VkDevice device, uint32_t size, uint32_t type, VkMemoryPropertyFlags properties, VkDeviceMemory& memory);
};
