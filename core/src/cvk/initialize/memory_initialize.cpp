#include "cvk/initialize/memory_initialize.h"

VkResult __cvk::create_memory(const VkPhysicalDeviceMemoryProperties& all_properties, VkDevice device, uint32_t size, uint32_t type, VkMemoryPropertyFlags properties, VkDeviceMemory& memory)
{    
    uint32_t memory_type_index;
    get_memory_type(all_properties, type, properties, memory_type_index);
    return create_memory(device, size, memory_type_index, memory);
}

void __cvk::destroy_memory(VkDevice device, VkDeviceMemory memory)
{
    vkFreeMemory(device, memory, nullptr);
}

VkResult __cvk::create_memory(VkDevice device, uint32_t size, uint32_t type_index, VkDeviceMemory& memory)
{
	VkMemoryAllocateInfo memory_allocate_info{};
	memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memory_allocate_info.allocationSize = size;
	memory_allocate_info.memoryTypeIndex = type_index;
	return vkAllocateMemory(device, &memory_allocate_info, nullptr, &memory);
}

void __cvk::get_memory_type(const VkPhysicalDeviceMemoryProperties& all_properties, uint32_t type, VkMemoryPropertyFlags properties, uint32_t& index)
{
    for (uint32_t i = 0; i < all_properties.memoryTypeCount; i++)
    {
        if ((type & 1) == 1)
        {
            if ((all_properties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                index = i;
                return;
            }
        }
        type >>= 1;
    }
}