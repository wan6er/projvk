#include "cvk/memory.h"
#include "cvk/initialize/memory_initialize.h"

namespace cvk
{


cvk::Memory::Memory(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryRequirements CONST_REFERENCE requirements, VkMemoryPropertyFlags property)
{
    __cvk::get_default_memory_allocate_info(_device, properties, requirements, property, allocate_info());
}

cvk::Memory::Memory(uint32_t size, uint32_t type_index)
{
    __cvk::get_default_memory_allocate_info(_device, size, type_index, allocate_info());
}

cvk::Memory::Memory(VkDeviceMemory CONST_REFERENCE memory) :
    utils::BaseObj<VkDeviceMemory>(memory)
{
}

cvk::Memory::~Memory()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

cvk::Memory::operator VkDeviceMemory CONST_REFERENCE () const
{
    return object();
}

VkResult cvk::Memory::allocate(VkDevice device)
{
    _device = device;
    return __cvk::alloc_memory(_device, _info, object());
}

auto cvk::Memory::allocate_info() -> VkMemoryAllocateInfo&
{
    return _info;
}

void cvk::Memory::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::free_memory(_device, object());
    }
}

void cvk::Memory::unmap()
{
    __cvk::unmap_memory(_device, object());
}

};