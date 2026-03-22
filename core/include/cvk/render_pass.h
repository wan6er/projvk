#pragma once

#include "cvk/vk_header.h"
#include "utils/base_multi_attach.h"
#include "cvk/pipe/base_render_pass.h"
#include "cvk/pipe/subpass.h"
#include "cvk/pipe/subpass_dependency.h"

namespace cvk
{
    
    class CVK_API RenderPass :
        public BaseRenderPass,
        public utils::BaseMultipleAttaches<Subpass, SubpassDependency>
    {
    public:
        RenderPass(VkDevice device);
        virtual ~RenderPass();

        virtual VkResult create();

        auto add_subpass(VkPipelineBindPoint bind_point) -> Subpass&;
        auto add_subpass_dependency(uint32_t src_subpass, uint32_t dst_subpass) -> SubpassDependency&;
        auto add_attachment(VkFormat format, VkImageLayout final_layout) -> RenderPass&;
        auto add_attachment(
            VkFormat format,
            VkImageLayout initial_layout,
            VkImageLayout final_layout,
            VkAttachmentLoadOp load_op,
            VkAttachmentStoreOp store_op,
            VkAttachmentLoadOp stencil_load_op = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            VkAttachmentStoreOp stencil_store_op = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT) -> RenderPass&;

    private:

    };

} // namespace cvk
