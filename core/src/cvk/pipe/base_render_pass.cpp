#include "cvk/pipe/base_render_pass.h"
#include "cvk/initialize/pipe_initialize.h"

namespace cvk
{

BaseRenderPass::BaseRenderPass(VkDevice device) :
    _device(device)
{
    setup_create_info();
}

BaseRenderPass::~BaseRenderPass()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

VkResult BaseRenderPass::create()
{
    setup_create_info();
    return __cvk::create_render_pass(_device, _info, object());
}

BaseRenderPass::operator VkRenderPass() const
{
    return object();
}

void BaseRenderPass::setup_create_info()
{
    __cvk::get_default_render_pass_create_info((*this), (*this), (*this), _info);
    // __cvk::get_default_render_pass_create_info(BaseMultipleAttachWrapper<VkAttachmentDescription>::get_attachments(), _subpass, _dependencies, _info);
}

// void BaseRenderPass::attach(VkAttachmentDescription CONST_LR attachment)
// {
//     _attachments.push_back(attachment);
// }

// void BaseRenderPass::attach(VkSubpassDescription CONST_LR subpass)
// {
//     _subpass.push_back(subpass);
// }

// void BaseRenderPass::attach(VkSubpassDependency CONST_LR subpass_dependency)
// {
//     _dependencies.push_back(subpass_dependency);
// }

auto BaseRenderPass::info() -> VkRenderPassCreateInfo&
{
    setup_create_info();
    return _info;
}

void BaseRenderPass::release()
{
    if (object() != VK_NULL_HANDLE) {
        __cvk::destroy_render_pass(_device, object());
    }
}

VkDevice BaseRenderPass::get_device()
{
    CVK_ASSERT(_device != VK_NULL_HANDLE);
    return _device;
}

};