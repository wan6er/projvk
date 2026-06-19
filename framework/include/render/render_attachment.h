#pragma once

#include "cvk/vk_header.h"

namespace cvk
{
    // Handle for one render pass attachment: ties a caller-assigned id to its
    // format and type. The id is the attachment index inside the render pass and
    // is provided by the caller, so it never relies on global/shared state.
    class Attachment
    {
    public:
        enum class Type
        {
            Color,
            Depth,
            Present,
        };

        Attachment() = default;

        explicit Attachment(uint32_t id) :
            _id(id)
        {
        }

        Attachment(uint32_t id, VkFormat format, Type type) :
            _id(id), _format(format), _type(type)
        {
        }

        uint32_t id() const { return _id; }
        VkFormat format() const { return _format; }
        Type type() const { return _type; }

    private:
        uint32_t _id = 0;
        VkFormat _format = VK_FORMAT_UNDEFINED;
        Type _type = Type::Color;
    };

    // An attachment referenced by a subpass, together with the layout it is used
    // in. Shared by both the input and output sides of a RenderElement.
    struct AttachmentRef
    {
        Attachment attachment;
        VkImageLayout layout;
    };
}
