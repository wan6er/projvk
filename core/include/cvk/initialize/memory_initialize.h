#pragma once

#include "cvk/vk_header.h"

namespace __cvk
{
    CVK_API void get_default_memory_allocate_info(VkDevice device, VkPhysicalDeviceMemoryProperties CONST_REFERENCE all_properties, VkMemoryRequirements CONST_REFERENCE requirements, VkMemoryPropertyFlags properties, VkMemoryAllocateInfo& info);
    CVK_API void get_default_memory_allocate_info(VkDevice device, VkPhysicalDeviceMemoryProperties CONST_REFERENCE all_properties, uint32_t size, uint32_t memory_type, VkMemoryPropertyFlags properties, VkMemoryAllocateInfo& info);
    CVK_API void get_default_memory_allocate_info(uint32_t size, uint32_t type_index, VkMemoryAllocateInfo& info);
    CVK_API VkResult alloc_memory(VkDevice device, VkPhysicalDeviceMemoryProperties CONST_REFERENCE all_properties, VkMemoryRequirements CONST_REFERENCE requirements, VkMemoryPropertyFlags properties, VkDeviceMemory &memory);
    CVK_API VkResult alloc_memory(VkDevice device, VkMemoryAllocateInfo CONST_REFERENCE info, VkDeviceMemory &memory);
    CVK_API void free_memory(VkDevice device, VkDeviceMemory memory);

    CVK_API void get_default_buffer_create_info(uint32_t size, VkBufferUsageFlags usage, std::vector<uint32_t> CONST_REFERENCE queue_families, VkBufferCreateInfo& info);
    CVK_API VkResult create_buffer(VkDevice device, VkBufferCreateInfo CONST_REFERENCE info, VkBuffer& buffer);
    CVK_API void destroy_buffer(VkDevice device, VkBuffer& buffer);

    CVK_API void get_memory_requirement(VkDevice device, VkBuffer buffer, VkMemoryRequirements& requirement);
    CVK_API void get_memory_requirement(VkDevice device, VkImage image, VkMemoryRequirements& requirement);
    CVK_API VkResult bind_memory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, uint32_t offset = 0);
    CVK_API VkResult bind_memory(VkDevice device, VkImage image, VkDeviceMemory memory, uint32_t offset = 0);

    template<class _Type>
    VkResult map_memory(VkDevice device, VkDeviceMemory memory, uint32_t size, _Type*& data, uint32_t offset = 0);
    CVK_API void unmap_memory(VkDevice device, VkDeviceMemory memory);
    
    CVK_API void get_memory_type(VkPhysicalDeviceMemoryProperties CONST_REFERENCE all_properties, uint32_t type, VkMemoryPropertyFlags properties, uint32_t &index);
};

#include "memory_initialize.inl"
