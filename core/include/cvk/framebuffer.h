#pragma once

#include "cvk/vk_header.h"
#include "cvk/image/image_view.h"
#include "utils/base_obj.h"
#include "utils/base_multi_attach.h"

namespace cvk
{
    class CVK_API Framebuffer :
        public utils::BaseObj<VkFramebuffer>,
        public utils::BaseMultipleAttaches<VkImageView>
    {
    public:
        Framebuffer(VkRenderPass renderpass, uint32_t width, uint32_t height);
        virtual ~Framebuffer();

        VkResult create(VkDevice device);

        operator VkFramebuffer CONST_REFERENCE () const;
        
    protected:
        void release();

    private:
        VkFramebufferCreateInfo _create_info;
        VkDevice _device = VK_NULL_HANDLE;

    };
};