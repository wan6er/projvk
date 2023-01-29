#include "cvk/command/base_command.h"
#include "cvk/initialize/command_initialize.h"

namespace cvk
{

// primary command
BaseCommandPrimary::BaseCommandPrimary(VkCommandBuffer CONST_REFERENCE buf) :
    BaseCommand(buf)
{
}

void BaseCommandPrimary::begin_renderpass(VkRenderPassBeginInfo CONST_REFERENCE info) const
{
    __cvk::cmd_begin_renderpass(buffer, info, VK_SUBPASS_CONTENTS_INLINE);
}

void BaseCommandPrimary::exec_secondary_commands(std::vector<VkCommandBuffer> CONST_REFERENCE buffers) const
{
    __cvk::cmd_execute_command(buffer, buffers);
}

void BaseCommandPrimary::next_subpass() const
{
    __cvk::cmd_next_subpass(buffer, VK_SUBPASS_CONTENTS_INLINE);
}


// secondary command
BaseCommandSecondary::BaseCommandSecondary(VkCommandBuffer CONST_REFERENCE buf) :
    BaseCommand(buf)
{
}

void BaseCommandSecondary::begin_renderpass(VkRenderPassBeginInfo CONST_REFERENCE info) const
{
    __cvk::cmd_begin_renderpass(buffer, info, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
}

void BaseCommandSecondary::next_subpass() const
{
    __cvk::cmd_next_subpass(buffer, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
}

// general command
BaseCommand::BaseCommand(VkCommandBuffer CONST_REFERENCE buffer) :
    buffer(buffer)
{
}

void BaseCommand::bind_pipeline(VkPipelineBindPoint bind_point, VkPipeline pipeline) const
{
    __cvk::cmd_bind_pipeline(buffer, bind_point, pipeline);
}

void BaseCommand::bind_descriptor_sets(VkPipelineBindPoint bind_point, VkPipelineLayout layout, std::vector<VkDescriptorSet> CONST_REFERENCE sets) const
{
    __cvk::cmd_bind_descriptor_sets(buffer, bind_point, layout, sets);
}

void BaseCommand::bind_vertex_buffers(std::vector<VkBuffer> CONST_REFERENCE vertex_buffers, std::vector<VkDeviceSize> CONST_REFERENCE offsets) const
{
    __cvk::cmd_bind_vertex_buffers(buffer, vertex_buffers, offsets);
}

void BaseCommand::bind_vertex_buffer(VkBuffer CONST_REFERENCE vertex_buffer) const
{
    __cvk::cmd_bind_vertex_buffers(buffer, { vertex_buffer }, { 0 });
}

void BaseCommand::bind_index_buffer(VkIndexType type, VkBuffer index_buffer) const
{
    __cvk::cmd_bind_index_buffer(buffer, type, index_buffer);
}

void BaseCommand::set_viewport(std::vector<VkViewport> CONST_REFERENCE viewports) const
{
    __cvk::cmd_set_viewport(buffer, viewports);
}

void BaseCommand::set_scissor(std::vector<VkRect2D> CONST_REFERENCE scissors) const
{
    __cvk::cmd_set_scissor(buffer, scissors);
}

void BaseCommand::draw(uint32_t vertex_count) const
{
    __cvk::cmd_draw(buffer, vertex_count);
}

void BaseCommand::draw_indexed(uint32_t index_count, uint32_t first_index) const
{
    __cvk::cmd_draw_indexed(buffer, index_count, first_index);
}

void BaseCommand::copy_buffer(VkBuffer src, VkBuffer dst, std::vector<VkBufferCopy> CONST_REFERENCE offset) const
{
    __cvk::cmd_copy_buffer(buffer, src, dst, offset);
}

void BaseCommand::copy_buffer_image(VkBuffer src, VkImage dst, std::vector<VkBufferImageCopy> CONST_REFERENCE offset) const
{
    __cvk::cmd_copy_buffer_to_image(buffer, src, dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, offset);
}

BaseBarrier BaseCommand::set_barrier(VkPipelineStageFlags src, VkPipelineStageFlags dst) const
{
    BaseBarrier barrier(buffer);
    barrier.src = src;
    barrier.dst = dst;
    return barrier;
}

void BaseCommand::end_renderpass() const
{
    __cvk::cmd_end_renderpass(buffer);
}

} // namespace cvk
