#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"
#include "utils/base_multi_attach.h"

namespace cvk
{
    class CVK_API ColorAttachment : 
        public utils::BaseMultipleAttaches<VkAttachmentReference>
    {
    protected:
        virtual auto color_attachments() const -> std::vector<VkAttachmentReference> CONST_REFERENCE;

    };


    class CVK_API DepthAttachment
    {
    public:
        virtual void attach(VkAttachmentReference CONST_REFERENCE reference) final;

    protected:
        virtual auto depth_attachments() const -> VkAttachmentReference CONST_REFERENCE;

    protected:
        virtual auto is_attached() const -> bool final;

    private:
        bool _attached = false;
        VkAttachmentReference _reference;
    };
    

    class CVK_API InputAttachment : 
        public utils::BaseMultipleAttaches<VkAttachmentReference>
    {
    protected:
        virtual auto input_attachments() const -> std::vector<VkAttachmentReference> CONST_REFERENCE;

    };

    class CVK_API Subpass :
        private ColorAttachment,
        private InputAttachment,
        private DepthAttachment
    {
    public:
        explicit Subpass(VkPipelineBindPoint bind_point);

        virtual auto color() -> ColorAttachment& final;
        virtual auto depth() -> DepthAttachment& final;
        virtual auto input() -> InputAttachment& final;

        VkSubpassDescription CONST_REFERENCE get_description();
        operator VkSubpassDescription CONST_REFERENCE();

    private:
        VkPipelineBindPoint _bind_point;
        VkSubpassDescription _description;
    };
};

// #include "render_pass.inl"