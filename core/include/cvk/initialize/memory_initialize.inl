#pragma once

#include <assert.h>

template<class _Type>
VkResult __cvk::map_memory(VkDevice device, VkDeviceMemory memory, uint32_t size, _Type*& data, uint32_t offset)
{
    assert(device);
    assert(memory);
    return vkMapMemory(device, memory, offset, size, 0, (void**)&data);
}
