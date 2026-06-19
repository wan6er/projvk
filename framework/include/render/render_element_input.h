#pragma once

#include "cvk/vk_header.h"
#include "render/render_attachment.h"

#include <vector>

namespace cvk
{
    // The input attachments read by one subpass.
    class RenderElementInput
    {
    public:
        RenderElementInput& add(uint32_t id,
            VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            _inputs.push_back({ Attachment(id), layout });
            return *this;
        }

        RenderElementInput& add(AttachmentRef CONST_REFERENCE ref,
            VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            _inputs.push_back({ ref.attachment, layout });
            return *this;
        }

        std::vector<AttachmentRef> CONST_REFERENCE inputs() const { return _inputs; }

    private:
        std::vector<AttachmentRef> _inputs;
    };
}
