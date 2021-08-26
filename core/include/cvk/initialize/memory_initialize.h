#pragma once

#include "cvk/vk_header.h"

namespace __cvk
{
    VkResult create_memory(const VkPhysicalDeviceMemoryProperties& all_properties, VkDevice device, uint32_t size, uint32_t type, VkMemoryPropertyFlags properties, VkDeviceMemory& memory);
    VkResult create_memory(VkDevice device, uint32_t size, uint32_t type_index, VkDeviceMemory &memory);
    void destroy_memory(VkDevice device, VkDeviceMemory memory);
    
    void get_memory_type(const VkPhysicalDeviceMemoryProperties &all_properties, uint32_t type, VkMemoryPropertyFlags properties, uint32_t &index);

};
