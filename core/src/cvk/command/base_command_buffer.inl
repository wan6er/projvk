#pragma once
#include "cvk/initialize/command_initialize.h"

namespace cvk
{

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
VkResult BaseCommandBuffer<_CmdType>::begin(VkCommandBufferUsageFlags usage)
{
    CVK_ASSERT(object() != VK_NULL_HANDLE);
    return __cvk::begin_command_buffer(object(), usage);
}

template<class _CmdType>
void BaseCommandBuffer<_CmdType>::end()
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

} // namespace cvk
