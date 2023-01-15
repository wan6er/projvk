#include "cvk/buffer.h"
#include "cvk/initialize/memory_initialize.h"

namespace cvk
{

Buffer::Buffer(VkDevice device) :
    _device(device)
{
}

Buffer::Buffer(VkBuffer buffer) :
    utils::BaseObj<VkBuffer>(buffer)
{
}

Buffer::~Buffer()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

auto Buffer::get_memory_requirement() const -> VkMemoryRequirements
{
    CVK_ASSERT(object() != VK_NULL_HANDLE);
    VkMemoryRequirements requirement;
    __cvk::get_memory_requirement(_device, object(), requirement);
    return requirement;
}

Buffer::operator VkBuffer() const
{
    CVK_ASSERT(object() != VK_NULL_HANDLE);
    return object();
}

auto Buffer::info() -> VkBufferCreateInfo&
{
    return _info;
}

VkResult Buffer::create(uint32_t size, VkBufferUsageFlags usage)
{
    __cvk::get_default_buffer_create_info(size, usage, *this, _info);
    return __cvk::create_buffer(_device, _info, object());
}

void Buffer::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::destroy_buffer(_device, object());
    }
}

};
