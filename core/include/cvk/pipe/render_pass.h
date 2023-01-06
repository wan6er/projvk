#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"
#include "utils/base_multi_attach.h"


namespace cvk
{
    class CVK_API RenderPass : 
        protected utils::BaseObj<VkRenderPass>, 
        public utils::BaseMultipleAttaches<VkAttachmentDescription, VkSubpassDescription, VkSubpassDependency>
    {
    public:
        RenderPass(VkDevice device);
        RenderPass(RenderPass CONST_REFERENCE) = default;
        ~RenderPass();

        VkResult create();
        operator VkRenderPass() const;

        auto info() -> VkRenderPassCreateInfo&;

    protected:
        void setup_create_info();
        void release();

    private:
        VkDevice _device = VK_NULL_HANDLE;
        VkRenderPassCreateInfo _info;
        std::vector<VkSubpassDescription> _subpass;

    };
};

#include "render_pass.inl"