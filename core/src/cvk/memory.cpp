#include "cvk/memory.h"

#include "cvk/initialize/memory_initialize.h"

cvk::Memory::Memory(VkDevice device, uint32_t size, uint32_t type_index) :
    _device(device)
{
    __cvk::create_memory(_device, size, type_index, object());
}

cvk::Memory::operator VkDeviceMemory() const
{
    return object();
}

void cvk::Memory::release()
{
    __cvk::destroy_memory(_device, object());
}
