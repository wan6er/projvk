#pragma once

#include "cvk/vk_header.h"

namespace __cvk
{

    CVK_API void get_default_framebuffer_create_info(VkRenderPass renderpass, std::vector<VkImageView> CONST_REFERENCE attachments, uint32_t width, uint32_t height, VkFramebufferCreateInfo& create_info);
    CVK_API VkResult create_framebuffer(VkDevice device, VkFramebufferCreateInfo CONST_REFERENCE create_info, VkFramebuffer& framebuffer);
    CVK_API void destroy_framebuffer(VkDevice device, VkFramebuffer framebuffer);

};