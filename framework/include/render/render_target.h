#pragma once

#include "cvk/vk_header.h"
#include "cvk/render_pass.h"
#include "render/render_attachment.h"
#include "render/render_element.h"

#include <vector>

namespace cvk
{
    // One render pass: creates and holds its subpass elements, and builds a
    // cvk::RenderPass from their attachment declarations. Attachment ids are
    // caller-assigned and used directly as the attachment indices.
    class RenderTarget
    {
    public:
        explicit RenderTarget()
        {
        }

        // Create a subpass element owned by this target.
        RenderElement& add_element(VkPipelineBindPoint bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS)
        {
            return _elements.emplace_back(bind_point);
        }

        // Declare a dependency between two subpasses (by element index).
        RenderTarget& add_dependency(uint32_t src, uint32_t dst,
            VkPipelineStageFlags src_stage, VkAccessFlags src_access,
            VkPipelineStageFlags dst_stage, VkAccessFlags dst_access)
        {
            _dependencies.push_back({ src, dst, src_stage, src_access, dst_stage, dst_access });
            return *this;
        }

        // Configure attachments, subpasses and dependencies on render_pass.
        void setup(RenderPass& render_pass);

    private:
        struct Dependency
        {
            uint32_t src;
            uint32_t dst;
            VkPipelineStageFlags src_stage;
            VkAccessFlags src_access;
            VkPipelineStageFlags dst_stage;
            VkAccessFlags dst_access;
        };

        static VkImageLayout final_layout(Attachment::Type type);

        std::vector<RenderElement> _elements;
        std::vector<Dependency> _dependencies;
    };
}
