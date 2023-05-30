#include "cvk/memory/memory.h"
#include "cvk/initialize/memory_initialize.h"

#include <memory.h>

namespace cvk
{

Memory::Memory(VkDevice device, void CONST_PTR next) :
    _BaseMemory(device)
{
    _info.pNext = next;
}

Memory::Memory(VkDevice device, VkDeviceMemory CONST_REFERENCE memory) :
    _BaseMemory(device, memory)
{
}

Memory::~Memory()
{
    if (isolated() && !is_cloned()) {
        deallocate();
    }
}

Memory::operator VkDeviceMemory CONST_REFERENCE () const
{
    return object();
}

VkResult Memory::bind(VkBuffer buffer, uint32_t offset)
{
    assert(object() != VK_NULL_HANDLE);
    
    return __cvk::bind_memory(device(), buffer, object(), offset);
}

VkResult Memory::bind(VkImage image, uint32_t offset)
{
    assert(object() != VK_NULL_HANDLE);
    
    return __cvk::bind_memory(device(), image, object(), offset);
}

VkResult Memory::allocate(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryRequirements CONST_REFERENCE requirements, VkMemoryPropertyFlags property)
{
    if (!object()) {
        __cvk::get_memory_allocate_info(device(), properties, requirements, property, allocate_info());
        return __cvk::alloc_memory(device(), _info, object());
    }
    return VK_SUCCESS;
}

VkResult Memory::allocate(uint32_t size, uint32_t type_index)
{
    if (!object()) {
        __cvk::get_default_memory_allocate_info(size, type_index, allocate_info());
        return __cvk::alloc_memory(device(), _info, object());
    }
    return VK_SUCCESS;
}

auto Memory::allocate_info() const -> VkMemoryAllocateInfo CONST_REFERENCE
{
    return _info;
}

auto Memory::allocate_info() -> VkMemoryAllocateInfo&
{
    return _info;
}

void Memory::deallocate()
{
    if (device() != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::free_memory(device(), object());
    }
}

VkResult Memory::upload(void CONST_PTR data, size_t size, size_t offset)
{
    void* device_ptr = nullptr;
    VkResult result = map(device_ptr, offset);
    if (result == VK_SUCCESS) {
        ::memcpy(device_ptr, data, size);
        unmap();
    }
    return result;
}

void Memory::unmap()
{
    __cvk::unmap_memory(device(), object());
}

};