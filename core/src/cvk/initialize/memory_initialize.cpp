#include "cvk/initialize/memory_initialize.h"



namespace __cvk
{

CVK_API void get_default_memory_allocate_info(VkDevice device, VkPhysicalDeviceMemoryProperties CONST_REFERENCE all_properties, VkMemoryRequirements CONST_REFERENCE requirements, VkMemoryPropertyFlags properties, VkMemoryAllocateInfo& info)
{    
    get_default_memory_allocate_info(device, all_properties, requirements.size, requirements.memoryTypeBits, properties, info);
}

CVK_API void get_default_memory_allocate_info(VkDevice device, VkPhysicalDeviceMemoryProperties CONST_REFERENCE all_properties, uint32_t size, uint32_t memory_type, VkMemoryPropertyFlags properties, VkMemoryAllocateInfo& info)
{    
    uint32_t memory_type_index = UINT32_MAX;
    get_memory_type(all_properties, memory_type, properties, memory_type_index);
    CVK_ASSERT(memory_type_index < all_properties.memoryTypeCount);

    // VkMemoryAllocateInfo info = {};
    get_default_memory_allocate_info(device, size, memory_type_index, info);
}


CVK_API void get_default_memory_allocate_info(VkDevice device, uint32_t size, uint32_t type_index, VkMemoryAllocateInfo& info)
{
	info = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = size,
        .memoryTypeIndex = type_index
    };
	// return vkAllocateMemory(device, &memory_allocate_info, nullptr, &memory);
}

CVK_API VkResult alloc_memory(VkDevice device, VkMemoryAllocateInfo CONST_REFERENCE info, VkDeviceMemory& memory)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
	return vkAllocateMemory(device, &info, nullptr, &memory);
}

CVK_API VkResult alloc_memory(VkDevice device, VkPhysicalDeviceMemoryProperties CONST_REFERENCE all_properties, VkMemoryRequirements CONST_REFERENCE requirements, VkMemoryPropertyFlags properties, VkDeviceMemory& memory)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    VkMemoryAllocateInfo info = {};
    get_default_memory_allocate_info(device, all_properties, requirements, properties, info);
	return alloc_memory(device, info, memory);
}

CVK_API void free_memory(VkDevice device, VkDeviceMemory memory)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(memory != VK_NULL_HANDLE);
    vkFreeMemory(device, memory, nullptr);
}

CVK_API void get_default_buffer_create_info(uint32_t size, VkBufferUsageFlags usage, std::vector<uint32_t> CONST_REFERENCE queue_families, VkBufferCreateInfo& info) {
    info = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .size = size,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = static_cast<uint32_t>(queue_families.size()),
        .pQueueFamilyIndices = queue_families.size() == 0 ? nullptr : queue_families.data()
    };
}

CVK_API void get_memory_requirement(VkDevice device, VkBuffer buffer, VkMemoryRequirements& requirement)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
    vkGetBufferMemoryRequirements(device, buffer, &requirement);
}

CVK_API void get_memory_requirement(VkDevice device, VkImage image, VkMemoryRequirements& requirement)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(image != VK_NULL_HANDLE);
    vkGetImageMemoryRequirements(device, image, &requirement);
}

CVK_API VkResult bind_memory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, uint32_t offset)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
    CVK_ASSERT(memory != VK_NULL_HANDLE);
    return vkBindBufferMemory(device, buffer, memory, offset);
}

CVK_API VkResult bind_memory(VkDevice device, VkImage image, VkDeviceMemory memory, uint32_t offset)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(image != VK_NULL_HANDLE);
    CVK_ASSERT(memory != VK_NULL_HANDLE);
    return vkBindImageMemory(device, image, memory, offset);
}

CVK_API VkResult create_buffer(VkDevice device, VkBufferCreateInfo CONST_REFERENCE info, VkBuffer& buffer) 
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    return vkCreateBuffer(device, &info, nullptr, &buffer);
}

CVK_API void destroy_buffer(VkDevice device, VkBuffer& buffer)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
    vkDestroyBuffer(device, buffer, nullptr);
}

CVK_API void unmap_memory(VkDevice device, VkDeviceMemory memory)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(memory != VK_NULL_HANDLE);
    vkUnmapMemory(device, memory);
}

CVK_API void get_memory_type(VkPhysicalDeviceMemoryProperties CONST_REFERENCE all_properties, uint32_t type, VkMemoryPropertyFlags properties, uint32_t& index)
{
    for (uint32_t i = 0; i < all_properties.memoryTypeCount; i++)
    {
        if ((type & 1) == 1)
        {
            if ((all_properties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                index = i;
                break;
            }
        }
        type >>= 1;
    }
}

};