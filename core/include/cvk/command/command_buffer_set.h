#pragma once

#include "base_command_buffer_set.h"

namespace cvk
{
    
    
// template<class _DerivedCmd, VkCommandBufferLevel _Level>
// class BaseSecondaryCommandBufferSet :
//     public BaseCommandBufferSet
// {
// public:
//     BaseSecondaryCommandBufferSet(VkDevice device, VkCommandPool pool, VkRenderPass renderpass, VkFramebuffer framebuffer, size_t size);

//     auto operator[](size_t index) const -> _DerivedCmd CONST_REFERENCE;

// private:
//     VkRenderPass _renderpass;
//     VkFramebuffer _framebuffer;
//     VkCommandBufferInheritanceInfo _inheritance_info = {};

// };


} // namespace cvk

#include "command_buffer_set.inl"