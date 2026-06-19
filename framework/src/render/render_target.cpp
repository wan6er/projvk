#include "render/render_target.h"

#include <map>

namespace cvk
{
    RenderPass RenderTarget::create()
    {
        RenderPass render_pass(_device);

        // Unique attachments keyed by id; the ordered map registers them in
        // id order, so the attachment index matches its id.
        std::map<uint32_t, Attachment> attachments;
        for (auto CONST_REFERENCE element : _elements) {
            for (auto CONST_REFERENCE color : element.output().colors()) {
                attachments[color.attachment.id()] = color.attachment;
            }
            if (element.output().has_depth()) {
                attachments[element.output().depth().attachment.id()] = element.output().depth().attachment;
            }
        }
        for (auto CONST_REFERENCE entry : attachments) {
            render_pass.add_attachment(entry.second.format(), final_layout(entry.second.type()));
        }

        for (auto CONST_REFERENCE element : _elements) {
            Subpass& subpass = render_pass.add_subpass(element.bind_point());
            for (auto CONST_REFERENCE input : element.input().inputs()) {
                subpass.add_input(input.attachment.id(), input.layout);
            }
            for (auto CONST_REFERENCE color : element.output().colors()) {
                subpass.add_color(color.attachment.id(), color.layout);
            }
            if (element.output().has_depth()) {
                subpass.set_depth(element.output().depth().attachment.id(), element.output().depth().layout);
            }
        }

        for (auto CONST_REFERENCE dependency : _dependencies) {
            render_pass.add_subpass_dependency(dependency.src, dependency.dst)
                .set_src(dependency.src_stage, dependency.src_access)
                .set_dst(dependency.dst_stage, dependency.dst_access);
        }

        render_pass.create();
        return render_pass;
    }

    VkImageLayout RenderTarget::final_layout(Attachment::Type type)
    {
        switch (type) {
        case Attachment::Type::Present: return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        case Attachment::Type::Depth:   return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        default:                        return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }
    }
}
