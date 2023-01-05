#pragma once

#include "cvk/vk_header.h"

namespace cvk
{
    
    struct CVK_API BaseCommand
    {
        VkCommandBuffer CONST_REFERENCE buffer;

        BaseCommand(VkCommandBuffer CONST_REFERENCE buffer);

        void bind_pipeline(VkPipelineBindPoint bind_point, VkPipeline pipeline) const;
        void bind_descriptor_sets(VkPipelineBindPoint bind_point, VkPipelineLayout layout, std::vector<VkDescriptorSet> CONST_REFERENCE sets) const;
        void bind_vertex_buffers(std::vector<VkBuffer> CONST_REFERENCE vertex_buffers) const;
        void bind_index_buffer(VkIndexType type, VkBuffer index_buffer) const;
        void set_viewport(std::vector<VkViewport> CONST_REFERENCE viewports) const;
        void set_scissor(std::vector<VkRect2D> CONST_REFERENCE scissors) const;
        void draw(uint32_t vertex_count) const;
        void draw_indexed(uint32_t index_count, uint32_t first_index = 0) const;

        void end_renderpass() const;
    };

    struct CVK_API CommandPrimary : public BaseCommand
    {
        CommandPrimary(VkCommandBuffer CONST_REFERENCE buffer);
        
        void begin_renderpass(VkRenderPassBeginInfo CONST_REFERENCE info) const;
        void exec_secondary_commands(std::vector<VkCommandBuffer> CONST_REFERENCE buffers) const;
    };

    struct CVK_API CommandSecondary : public BaseCommand
    {
        CommandSecondary(VkCommandBuffer CONST_REFERENCE buffer);

        void begin_renderpass(VkRenderPassBeginInfo CONST_REFERENCE info) const;

    };


} // namespace cvk
