#pragma once

#include "cvk/initialize/command_initialize.h"

namespace cvk
{

// template<class _DerivedCmd, VkCommandBufferLevel _Level>
// BaseSecondaryCommandBufferSet<_DerivedCmd, _Level>::BaseSecondaryCommandBufferSet(VkDevice device, VkCommandPool pool, VkRenderPass renderpass, VkFramebuffer framebuffer, size_t size) :
//     BaseCommandBufferSet(device, pool, _Level, size),
//     _renderpass(renderpass),
//     _framebuffer(framebuffer)
// {
//     // alloc_info.pNext = &_inheritance_info;
//     // __cvk::get_default_command_buffer_inheritance_allocate_info(_renderpass, _framebuffer, subpass, _inheritance_info);
    
// }

// template<class _DerivedCmd, VkCommandBufferLevel _Level>
// auto BaseSecondaryCommandBufferSet<_DerivedCmd, _Level>::operator[](size_t index) const -> _DerivedCmd CONST_REFERENCE
// {
//     return _DerivedCmd(index);
// }

}