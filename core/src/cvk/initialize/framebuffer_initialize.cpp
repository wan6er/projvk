#include "cvk/initialize/framebuffer_initialize.h"
#include "utils/vector_util.h"



namespace __cvk
{

CVK_API void get_default_framebuffer_create_info(VkRenderPass renderpass, std::vector<VkImageView> CONST_REFERENCE attachments, uint32_t width, uint32_t height, VkFramebufferCreateInfo& create_info)
{
    create_info = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .pNext = NULL,
        .renderPass = renderpass,
        .width = width,
        .height = height,
        .layers = 1,
    };
    utils::vector_fill_info(attachments, create_info.attachmentCount, create_info.pAttachments);
}

CVK_API VkResult create_framebuffer(VkDevice device, VkFramebufferCreateInfo CONST_REFERENCE create_info, VkFramebuffer& framebuffer)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    return vkCreateFramebuffer(device, &create_info, nullptr, &framebuffer);
}

CVK_API void destroy_framebuffer(VkDevice device, VkFramebuffer framebuffer)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(framebuffer != VK_NULL_HANDLE);
    vkDestroyFramebuffer(device, framebuffer, nullptr);
}

};