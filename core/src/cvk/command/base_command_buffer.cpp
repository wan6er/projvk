#include "cvk/command/base_command_buffer.h"
#include "cvk/command/base_command.h"
#include "cvk/initialize/command_initialize.h"
// #include "base_command_buffer.inl"

namespace cvk
{

// BaseCommandBuffer::BaseCommandBuffer(VkCommandBuffer CONST_REFERENCE buffer) :
//     _CmdBufBase(buffer)
// {
//     CVK_ASSERT(buffer != VK_NULL_HANDLE);
// }


BaseCommandBuffer::BaseCommandBuffer(VkDevice device, VkCommandPool pool) :
    _CmdBufBase(device),
    _pool(pool)
{
}

BaseCommandBuffer::~BaseCommandBuffer()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

VkResult BaseCommandBuffer::create()
{
    CVK_ASSERT(device() != VK_NULL_HANDLE);
    CVK_ASSERT(_pool != VK_NULL_HANDLE);
    return __cvk::alloc_command_buffer(device(), _allocate_info, object());
}

void BaseCommandBuffer::end() const
{
    CVK_ASSERT(object() != VK_NULL_HANDLE);
    __cvk::end_command_buffer(object());
}

BaseCommandBuffer::operator VkCommandBuffer() const
{
    CVK_ASSERT(object() != VK_NULL_HANDLE);
    return object();
}

void BaseCommandBuffer::release()
{
    if (device() != VK_NULL_HANDLE && _pool != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::free_command_buffers(device(), _pool, { object() });
    }
}



CommandBuffer::CommandBuffer(VkDevice device, VkCommandPool pool) :
    BaseCommandBuffer(device, pool)
{
    __cvk::get_default_command_buffers_allocate_info(pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1, this->_allocate_info);
}

// CommandBuffer::CommandBuffer(VkCommandBuffer CONST_REFERENCE buffer) :
//     BaseCommandBuffer(buffer)
// {
// }

VkResult CommandBuffer::begin(VkCommandBufferUsageFlags usage) const
{
    CVK_ASSERT(this->object() != VK_NULL_HANDLE);
    return __cvk::begin_command_buffer(this->object(), usage);
}

CommandBufferSecondary::CommandBufferSecondary(VkDevice device, VkCommandPool pool) :
    BaseCommandBuffer(device, pool)
{
    __cvk::get_default_command_buffers_allocate_info(this->_pool, VK_COMMAND_BUFFER_LEVEL_SECONDARY, 1, this->_allocate_info);
}

// CommandBufferSecondary::CommandBufferSecondary(VkCommandBuffer CONST_REFERENCE buffer) :
//     BaseCommandBuffer(buffer)
// {
// }
        
VkResult CommandBufferSecondary::begin(VkRenderPass renderpass, VkFramebuffer framebuffer, uint32_t subpass, VkCommandBufferUsageFlags usage) const
{
    VkCommandBufferInheritanceInfo info = {};
    __cvk::get_default_command_buffer_inheritance_allocate_info(renderpass, framebuffer, subpass, info);
    return __cvk::begin_command_buffer_secondary(this->object(), info, usage);
}

} // namespace cvk
