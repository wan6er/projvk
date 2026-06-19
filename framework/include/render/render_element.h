#pragma once

#include "cvk/vk_header.h"
#include "render/render_element_input.h"
#include "render/render_element_output.h"

namespace cvk
{
    // One subpass: a bind point plus its input and output attachments.
    class RenderElement
    {
    public:
        explicit RenderElement(VkPipelineBindPoint bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS) :
            _bind_point(bind_point)
        {
        }

        RenderElementInput& input() { return _input; }
        RenderElementOutput& output() { return _output; }

        RenderElementInput CONST_REFERENCE input() const { return _input; }
        RenderElementOutput CONST_REFERENCE output() const { return _output; }
        VkPipelineBindPoint bind_point() const { return _bind_point; }

    private:
        VkPipelineBindPoint _bind_point;
        RenderElementInput _input;
        RenderElementOutput _output;
    };
}
