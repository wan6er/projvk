#include "cvk/buffer.h"
#include "cvk/initialize/memory_initialize.h"



namespace cvk
{


cvk::Buffer::Buffer(uint32_t size, VkBufferUsageFlags usage)
{
    __cvk::get_default_buffer_create_info(size, usage, {}, _info);
}

cvk::Buffer::Buffer(VkBuffer buffer) :
    utils::BaseObj<VkBuffer>(buffer)
{
}

cvk::Buffer::~Buffer()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

auto cvk::Buffer::get_memory_requirement() const -> VkMemoryRequirements
{
    CVK_ASSERT(object() != VK_NULL_HANDLE);
    VkMemoryRequirements requirement;
    __cvk::get_memory_requirement(_device, object(), requirement);
    return requirement;
}

cvk::Buffer::operator VkBuffer() const
{
    CVK_ASSERT(object() != VK_NULL_HANDLE);
    return object();
}

auto cvk::Buffer::info() -> VkBufferCreateInfo&
{
    return _info;
}

VkResult cvk::Buffer::create(VkDevice device)
{
    _device = device;
    return __cvk::create_buffer(_device, _info, object());
}

void cvk::Buffer::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::destroy_buffer(_device, object());
    }
}

};
