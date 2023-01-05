#pragma once

#include "cvk/initialize/memory_initialize.h"

#include <assert.h>

template<class _Type>
VkResult cvk::Memory::bind(_Type&& obj, uint32_t offset)
{
    assert(_device != VK_NULL_HANDLE);
    assert(object() != VK_NULL_HANDLE);
    
    return __cvk::bind_memory(_device, obj, object(), offset);
}

template<class _Type>
VkResult cvk::Memory::map(_Type&& data, uint32_t offset)
{
    assert(_device != VK_NULL_HANDLE);
    assert(object() != VK_NULL_HANDLE);

    return __cvk::map_memory(_device, object(), _info.allocationSize, std::forward<_Type>(data), offset);
}
