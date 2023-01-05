#include "cvk/framebuffer.h"
#include "cvk/initialize/framebuffer_initialize.h"
#include "utils/vector_util.h"

namespace cvk
{

Framebuffer::Framebuffer(VkRenderPass renderpass, uint32_t width, uint32_t height)
{   
    __cvk::get_default_framebuffer_create_info(renderpass, {}, width, height, _create_info);
}

Framebuffer::~Framebuffer()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

VkResult Framebuffer::create(VkDevice dev)
{
    _device = dev;

    std::vector<VkImageView> CONST_REFERENCE image_views = *this;
    utils::vector_fill_info(image_views, _create_info.attachmentCount, _create_info.pAttachments);
    return __cvk::create_framebuffer(_device, _create_info, object());
}

Framebuffer::operator VkFramebuffer CONST_REFERENCE () const
{
    return object();
}

void Framebuffer::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::destroy_framebuffer(_device, object());
    }
}

};