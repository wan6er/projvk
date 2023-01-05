#include "cvk/pipe/render_pass.h"
#include "cvk/initialize/pipe_initialize.h"

namespace cvk
{

RenderPass::RenderPass()
{
    setup_create_info();
}

RenderPass::~RenderPass()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

VkResult RenderPass::create(VkDevice device)
{
    _device = device;
    setup_create_info();
    return __cvk::create_render_pass(_device, _info, object());
}

RenderPass::operator VkRenderPass() const
{
    return object();
}

void RenderPass::setup_create_info()
{
    __cvk::get_default_render_pass_create_info((*this), (*this), (*this), _info);
    // __cvk::get_default_render_pass_create_info(BaseMultipleAttachWrapper<VkAttachmentDescription>::get_attachments(), _subpass, _dependencies, _info);
}

// void RenderPass::attach(VkAttachmentDescription CONST_LR attachment)
// {
//     _attachments.push_back(attachment);
// }

// void RenderPass::attach(VkSubpassDescription CONST_LR subpass)
// {
//     _subpass.push_back(subpass);
// }

// void RenderPass::attach(VkSubpassDependency CONST_LR subpass_dependency)
// {
//     _dependencies.push_back(subpass_dependency);
// }

auto RenderPass::info() -> VkRenderPassCreateInfo&
{
    setup_create_info();
    return _info;
}

void RenderPass::release()
{
    if (object() != VK_NULL_HANDLE) {
        __cvk::destroy_render_pass(_device, object());
    }
}

};