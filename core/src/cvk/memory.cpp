#include "cvk/memory.h"
#include "cvk/initialize/memory_initialize.h"

namespace cvk
{

Memory::Memory(VkDevice device) :
    _device(device)
{
}

Memory::Memory(VkDeviceMemory CONST_REFERENCE memory) :
    utils::BaseObj<VkDeviceMemory>(memory)
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
    assert(_device != VK_NULL_HANDLE);
    assert(object() != VK_NULL_HANDLE);
    
    return __cvk::bind_memory(_device, buffer, object(), offset);
}

VkResult Memory::bind(VkImage image, uint32_t offset)
{
    assert(_device != VK_NULL_HANDLE);
    assert(object() != VK_NULL_HANDLE);
    
    return __cvk::bind_memory(_device, image, object(), offset);
}

VkResult Memory::allocate(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryRequirements CONST_REFERENCE requirements, VkMemoryPropertyFlags property)
{
    CVK_ASSERT(_device != VK_NULL_HANDLE);
    __cvk::get_default_memory_allocate_info(_device, properties, requirements, property, allocate_info());
    return __cvk::alloc_memory(_device, _info, object());
}

VkResult Memory::allocate(uint32_t size, uint32_t type_index)
{
    __cvk::get_default_memory_allocate_info(size, type_index, allocate_info());
    return __cvk::alloc_memory(_device, _info, object());
}

auto Memory::allocate_info() const -> VkMemoryAllocateInfo CONST_REFERENCE
{
    return _info;
}

auto Memory::allocate_info() -> VkMemoryAllocateInfo&
{
    return _info;
}

size_t Memory::get_size() const
{
    CVK_ASSERT(_info.allocationSize > 0);
    return _info.allocationSize;
}

void Memory::deallocate()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::free_memory(_device, object());
    }
}

VkResult Memory::upload(void CONST_PTR data, size_t size, size_t offset)
{
    void* device_ptr = nullptr;
    VkResult result = map(device_ptr, offset);
    if (result == VK_SUCCESS) {
        memcpy(device_ptr, data, size);
        unmap();
    }
    return result;
}

void Memory::unmap()
{
    __cvk::unmap_memory(_device, object());
}

};