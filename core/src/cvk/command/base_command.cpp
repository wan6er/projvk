#include "cvk/command/base_command.h"
#include "cvk/initialize/command_initialize.h"

namespace cvk
{

// general command
BaseCommand::BaseCommand(VkDevice device, VkCommandBuffer buffer) :
    _BaseCmd(device, buffer)
{
}

void CommandPrimary::begin_renderpass(VkRenderPassBeginInfo CONST_REFERENCE info) const
{
    __cvk::cmd_begin_renderpass(object(), info, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandPrimary::exec_secondary_commands(std::vector<VkCommandBuffer> CONST_REFERENCE buffers) const
{
    __cvk::cmd_execute_command(object(), buffers);
}

void CommandPrimary::next_subpass() const
{
    __cvk::cmd_next_subpass(object(), VK_SUBPASS_CONTENTS_INLINE);
}

void CommandSecondary::begin_renderpass(VkRenderPassBeginInfo CONST_REFERENCE info) const
{
    __cvk::cmd_begin_renderpass(object(), info, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
}

void CommandSecondary::next_subpass() const
{
    __cvk::cmd_next_subpass(object(), VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
}

void BaseCommand::bind_pipeline(VkPipelineBindPoint bind_point, VkPipeline pipeline) const
{
    __cvk::cmd_bind_pipeline(object(), bind_point, pipeline);
}

void BaseCommand::bind_descriptor_sets(VkPipelineBindPoint bind_point, VkPipelineLayout layout, std::vector<VkDescriptorSet> CONST_REFERENCE sets) const
{
    __cvk::cmd_bind_descriptor_sets(object(), bind_point, layout, sets);
}

void BaseCommand::bind_vertex_buffers(std::vector<VkBuffer> CONST_REFERENCE vertex_buffers, std::vector<VkDeviceSize> CONST_REFERENCE offsets) const
{
    __cvk::cmd_bind_vertex_buffers(object(), vertex_buffers, offsets);
}

void BaseCommand::bind_vertex_buffer(VkBuffer CONST_REFERENCE vertex_buffer) const
{
    __cvk::cmd_bind_vertex_buffers(object(), { vertex_buffer }, { 0 });
}

void BaseCommand::bind_index_buffer(VkIndexType type, VkBuffer index_buffer) const
{
    __cvk::cmd_bind_index_buffer(object(), type, index_buffer);
}

void BaseCommand::set_viewport(std::vector<VkViewport> CONST_REFERENCE viewports) const
{
    __cvk::cmd_set_viewport(object(), viewports);
}

void BaseCommand::set_scissor(std::vector<VkRect2D> CONST_REFERENCE scissors) const
{
    __cvk::cmd_set_scissor(object(), scissors);
}

void BaseCommand::draw(uint32_t vertex_count) const
{
    __cvk::cmd_draw(object(), vertex_count);
}

void BaseCommand::draw_indexed(uint32_t index_count, uint32_t first_index) const
{
    __cvk::cmd_draw_indexed(object(), index_count, first_index);
}

void BaseCommand::dispatch(uint32_t groupx, uint32_t groupy, uint32_t groupz) const
{
    __cvk::cmd_dispatch(object(), groupx, groupy, groupz);
}

void BaseCommand::copy_buffer(VkBuffer src, VkBuffer dst, std::vector<VkBufferCopy> CONST_REFERENCE offset) const
{
    __cvk::cmd_copy_buffer(object(), src, dst, offset);
}

void BaseCommand::copy_buffer_image(VkBuffer src, VkImage dst, std::vector<VkBufferImageCopy> CONST_REFERENCE offset) const
{
    __cvk::cmd_copy_buffer_to_image(object(), src, dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, offset);
}

void BaseCommand::copy_image(VkImage src, VkImage dst, VkImageAspectFlags aspect, VkExtent3D extent) const
{
    __cvk::cmd_copy_image(object(), aspect, src, dst, extent);
}

void BaseCommand::set_image_layout_barrier(VkImageLayout srclayout, VkImageLayout dstlayout, VkImage image, VkImageSubresourceRange range, VkPipelineStageFlags srcstage, VkPipelineStageFlags dststage) const
{
    __cvk::cmd_image_layout_barrier(object(), srclayout, dstlayout, image, range, srcstage, dststage);
}

void BaseCommand::build_acceleration_structure(VkAccelerationStructureBuildGeometryInfoKHR CONST_REFERENCE geoinfo, VkAccelerationStructureBuildRangeInfoKHR CONST_REFERENCE rangeinfo) const
{
    __cvk::cmd_build_acceleration_structure(device(), object(), geoinfo, rangeinfo);
}

BaseBarrier BaseCommand::set_barrier(VkPipelineStageFlags src, VkPipelineStageFlags dst) const
{
    BaseBarrier barrier(object());
    barrier.src = src;
    barrier.dst = dst;
    return barrier;
}

void BaseCommand::end_renderpass() const
{
    __cvk::cmd_end_renderpass(object());
}

} // namespace cvk
