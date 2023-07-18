#pragma once

#include "cvk/vk_header.h"
#include "utils/base_device_obj.h"
#include "base_barrier.h"

namespace cvk
{
    
    struct CVK_API BaseCommand : public utils::BaseDeviceObj<VkCommandBuffer>
    {
        using _BaseCmd = utils::BaseDeviceObj<VkCommandBuffer>;

        // VkCommandBuffer buffer;

        BaseCommand(VkDevice device, VkCommandBuffer buffer);

        void bind_pipeline(VkPipelineBindPoint bind_point, VkPipeline pipeline) const;
        void bind_descriptor_sets(VkPipelineBindPoint bind_point, VkPipelineLayout layout, std::vector<VkDescriptorSet> CONST_REFERENCE sets) const;
        void bind_vertex_buffers(std::vector<VkBuffer> CONST_REFERENCE vertex_buffers, std::vector<VkDeviceSize> CONST_REFERENCE offsets) const;
        void bind_vertex_buffer(VkBuffer CONST_REFERENCE vertex_buffer) const;
        void bind_index_buffer(VkIndexType type, VkBuffer index_buffer) const;
        void set_viewport(std::vector<VkViewport> CONST_REFERENCE viewports) const;
        void set_scissor(std::vector<VkRect2D> CONST_REFERENCE scissors) const;
        void draw(uint32_t vertex_count) const;
        void draw_indexed(uint32_t index_count, uint32_t first_index = 0) const;
        void dispatch(uint32_t groupx, uint32_t groupy, uint32_t groupz = 1) const;

        void copy_buffer(VkBuffer src, VkBuffer dst, std::vector<VkBufferCopy> CONST_REFERENCE offset) const;
        void copy_buffer_image(VkBuffer src, VkImage dst, std::vector<VkBufferImageCopy> CONST_REFERENCE offset) const;

        void copy_image(VkImage src, VkImage dst, VkImageAspectFlags aspect, VkExtent3D extent) const;
        void set_image_layout_barrier(VkImageLayout srclayout, VkImageLayout dstlayout, VkImage image, VkImageSubresourceRange range, VkPipelineStageFlags srcstage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VkPipelineStageFlags dststage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT) const;

        void build_acceleration_structure(VkAccelerationStructureBuildGeometryInfoKHR CONST_REFERENCE geoinfo, VkAccelerationStructureBuildRangeInfoKHR CONST_REFERENCE rangeinfo) const;

        BaseBarrier set_barrier(VkPipelineStageFlags src, VkPipelineStageFlags dst) const;

        void end_renderpass() const;
    };

    struct CVK_API CommandPrimary : public BaseCommand
    {
        using BaseCommand::BaseCommand;
        
        void begin_renderpass(VkRenderPassBeginInfo CONST_REFERENCE info) const;
        void exec_secondary_commands(std::vector<VkCommandBuffer> CONST_REFERENCE buffers) const;
        
        void next_subpass() const;
    };

    struct CVK_API CommandSecondary : public BaseCommand
    {
        using BaseCommand::BaseCommand;

        void begin_renderpass(VkRenderPassBeginInfo CONST_REFERENCE info) const;

        void next_subpass() const;
    };


} // namespace cvk
