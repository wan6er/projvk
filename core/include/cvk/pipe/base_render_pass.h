#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"
#include "utils/base_multi_attach.h"


namespace cvk
{
    class CVK_API BaseRenderPass : 
        protected utils::BaseObj<VkRenderPass>, 
        public utils::BaseMultipleAttaches<VkAttachmentDescription, VkSubpassDescription, VkSubpassDependency>
    {
    public:
        BaseRenderPass(VkDevice device);
        BaseRenderPass(BaseRenderPass CONST_REFERENCE) = default;
        ~BaseRenderPass();

        virtual VkResult create();
        operator VkRenderPass() const;

        auto info() -> VkRenderPassCreateInfo&;

    protected:
        void setup_create_info();
        void release();
        VkDevice get_device();

    private:
        VkDevice _device = VK_NULL_HANDLE;
        VkRenderPassCreateInfo _info;

    };
};

#include "base_render_pass.inl"