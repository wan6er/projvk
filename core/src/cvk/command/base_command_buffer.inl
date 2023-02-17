#pragma once
#include "cvk/initialize/command_initialize.h"

namespace cvk
{
    
template<class _CmdType>
BaseCommandBuffer<_CmdType>::BaseCommandBuffer(VkCommandBuffer CONST_REFERENCE buffer) :
    utils::BaseObj<VkCommandBuffer>(buffer),
    _cmd(buffer)
{
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
}


template<class _CmdType>
BaseCommandBuffer<_CmdType>::BaseCommandBuffer(VkDevice device, VkCommandPool pool) :
    _device(device),
    _pool(pool),
    _cmd(object())
{
}

template<class _CmdType>
BaseCommandBuffer<_CmdType>::~BaseCommandBuffer()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

template<class _CmdType>
VkResult BaseCommandBuffer<_CmdType>::create()
{
    CVK_ASSERT(_device != VK_NULL_HANDLE);
    CVK_ASSERT(_pool != VK_NULL_HANDLE);
    return __cvk::alloc_command_buffer(_device, _allocate_info, object());
}

template<class _CmdType>
void BaseCommandBuffer<_CmdType>::end() const
{
    CVK_ASSERT(object() != VK_NULL_HANDLE);
    __cvk::end_command_buffer(object());
}

template<class _CmdType>
auto BaseCommandBuffer<_CmdType>::cmd() const -> _CmdType CONST_REFERENCE
{
    CVK_ASSERT(object() != VK_NULL_HANDLE);
    return _cmd;
}

template<class _CmdType>
BaseCommandBuffer<_CmdType>::operator VkCommandBuffer CONST_REFERENCE () const
{
    CVK_ASSERT(object() != VK_NULL_HANDLE);
    return object();
}

template<class _CmdType>
void BaseCommandBuffer<_CmdType>::release()
{
    if (_device != VK_NULL_HANDLE && _pool != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::free_command_buffers(_device, _pool, { object() });
    }
}


template<class _CmdType>
BaseCommandBufferPrimary<_CmdType>::BaseCommandBufferPrimary(VkDevice device, VkCommandPool pool) :
    BaseCommandBuffer<_CmdType>(device, pool)
{
    __cvk::get_default_command_buffers_allocate_info(pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1, this->_allocate_info);
}

template<class _CmdType>
BaseCommandBufferPrimary<_CmdType>::BaseCommandBufferPrimary(VkCommandBuffer CONST_REFERENCE buffer) :
    BaseCommandBuffer<_CmdType>(buffer)
{
}

template<class _CmdType>
VkResult BaseCommandBufferPrimary<_CmdType>::begin(VkCommandBufferUsageFlags usage) const
{
    CVK_ASSERT(this->object() != VK_NULL_HANDLE);
    return __cvk::begin_command_buffer(this->object(), usage);
}

template<class _CmdType>
BaseCommandBufferSecondary<_CmdType>::BaseCommandBufferSecondary(VkDevice device, VkCommandPool pool) :
    BaseCommandBuffer<_CmdType>(device, pool)
{
    __cvk::get_default_command_buffers_allocate_info(this->_pool, VK_COMMAND_BUFFER_LEVEL_SECONDARY, 1, this->_allocate_info);
}

template<class _CmdType>
BaseCommandBufferSecondary<_CmdType>::BaseCommandBufferSecondary(VkCommandBuffer CONST_REFERENCE buffer) :
    BaseCommandBuffer<_CmdType>(buffer)
{

}
        
template<class _CmdType>
VkResult BaseCommandBufferSecondary<_CmdType>::begin(VkRenderPass renderpass, VkFramebuffer framebuffer, uint32_t subpass, VkCommandBufferUsageFlags usage) const
{
    VkCommandBufferInheritanceInfo info = {};
    __cvk::get_default_command_buffer_inheritance_allocate_info(renderpass, framebuffer, subpass, info);
    return __cvk::begin_command_buffer_secondary(this->object(), info, usage);
}


} // namespace cvk
