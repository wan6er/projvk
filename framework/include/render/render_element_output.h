#pragma once

#include "cvk/vk_header.h"
#include "render/render_attachment.h"

#include <optional>
#include <vector>

namespace cvk
{
    // The color/depth attachments written by one subpass.
    class RenderElementOutput
    {
    public:
        AttachmentRef add_color(uint32_t id, VkFormat format,
            VkImageLayout layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        {
            _colors.push_back({ Attachment(id, format, Attachment::Type::Color), layout });
            return _colors.back();
        }

        AttachmentRef add_present(uint32_t id, VkFormat format,
            VkImageLayout layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        {
            _colors.push_back({ Attachment(id, format, Attachment::Type::Present), layout });
            return _colors.back();
        }

        AttachmentRef set_depth(uint32_t id, VkFormat format,
            VkImageLayout layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
        {
            _depth = { Attachment(id, format, Attachment::Type::Depth), layout };
            return *_depth;
        }

        std::vector<AttachmentRef> CONST_REFERENCE colors() const { return _colors; }
        bool has_depth() const { return _depth.has_value(); }
        AttachmentRef CONST_REFERENCE depth() const { return *_depth; }

    private:
        std::vector<AttachmentRef> _colors;
        std::optional<AttachmentRef> _depth;
    };
}
