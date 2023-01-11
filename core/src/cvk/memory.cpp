#include "cvk/memory.h"
#include "cvk/initialize/memory_initialize.h"

namespace cvk
{


// Memory::Memory(VkDevice device, VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryRequirements CONST_REFERENCE requirements, VkMemoryPropertyFlags property) :
//     _device(device)
// {
//     __cvk::get_default_memory_allocate_info(_device, properties, requirements, property, allocate_info());
// }

// Memory::Memory(VkDevice device, uint32_t size, uint32_t type_index) :
//     _device(device)
// {
//     __cvk::get_default_memory_allocate_info(_device, size, type_index, allocate_info());
// }

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
        release();
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

auto Memory::allocate_info() -> VkMemoryAllocateInfo&
{
    return _info;
}

void Memory::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::free_memory(_device, object());
    }
}

void Memory::unmap()
{
    __cvk::unmap_memory(_device, object());
}

};