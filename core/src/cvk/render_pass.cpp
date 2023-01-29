#include "cvk/render_pass.h"
#include "cvk/initialize/pipe_initialize.h"

namespace cvk
{
    
RenderPass::RenderPass(VkDevice device) :
    BaseRenderPass(device)
{
}

RenderPass::~RenderPass()
{

}

VkResult RenderPass::create()
{
    std::vector<VkAttachmentDescription> CONST_REFERENCE attachments = *this;
    CVK_ASSERT(attachments.size() > 0);
    
    std::vector<Subpass> CONST_REFERENCE subpasses = *this;
    for (auto CONST_REFERENCE subpass : subpasses) {
        // subpass.update_description();
        BaseRenderPass::attaches(subpass.get_description());
    }
    std::vector<SubpassDependency> CONST_REFERENCE dependencies = *this;
    for (auto CONST_REFERENCE dependency : dependencies) {
        BaseRenderPass::attaches(static_cast<VkSubpassDependency>(dependency));
    }
    
    return BaseRenderPass::create();
}

auto RenderPass::add_subpass(VkPipelineBindPoint bind_point) -> Subpass&
{
    std::vector<Subpass>& subpasses = *this;
    return subpasses.emplace_back(bind_point);
}

auto RenderPass::add_subpass_dependency(uint32_t src_subpass, uint32_t dst_subpass) -> SubpassDependency&
{
    std::vector<SubpassDependency>& subpass_dependencies = *this;
    return subpass_dependencies.emplace_back(src_subpass, dst_subpass);
}

auto RenderPass::add_attachment(VkFormat format, VkImageLayout final_layout) -> RenderPass&
{
    VkAttachmentDescription attachment_desc = {};
    __cvk::get_default_attachment_description(format, final_layout, attachment_desc);
    BaseRenderPass::attaches(attachment_desc);
    return *this;
}
    
} // namespace cvk
