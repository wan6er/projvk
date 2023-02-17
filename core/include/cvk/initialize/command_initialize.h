#pragma once

#include "cvk/vk_header.h"
#include "utils/vector_util.h"

#include <vector>

namespace __cvk
{
    // command pool
    CVK_API void get_default_command_pool_create_info(uint32_t queue_index, VkCommandPoolCreateInfo& create_info);
    CVK_API VkResult create_command_pool(VkDevice device, VkCommandPoolCreateInfo CONST_REFERENCE create_info, VkCommandPool& command_pool);
    CVK_API void destroy_command_pool(VkDevice device, VkCommandPool command_pool);
    
    // command buffer
    CVK_API void get_default_command_buffer_inheritance_allocate_info(VkRenderPass renderpass, VkFramebuffer framebuffer, uint32_t subpass, VkCommandBufferInheritanceInfo& info);
    CVK_API void get_default_command_buffers_allocate_info(VkCommandPool pool, VkCommandBufferLevel level, uint32_t size, VkCommandBufferAllocateInfo& info);
    CVK_API VkResult append_alloc_command_buffers(VkDevice device, VkCommandBufferAllocateInfo CONST_REFERENCE info, std::vector<VkCommandBuffer>& buffers);
    CVK_API VkResult alloc_command_buffers(VkDevice device, VkCommandBufferAllocateInfo CONST_REFERENCE info, std::vector<VkCommandBuffer>& buffers);
    CVK_API VkResult alloc_command_buffer(VkDevice device, VkCommandBufferAllocateInfo CONST_REFERENCE info, VkCommandBuffer& buffer);
    CVK_API void free_command_buffers(VkDevice device, VkCommandPool command_pool, std::vector<VkCommandBuffer> CONST_REFERENCE buffers);
    CVK_API VkResult begin_command_buffer(VkCommandBuffer command_buffer, VkCommandBufferUsageFlags usage);
    CVK_API VkResult begin_command_buffer_secondary(VkCommandBuffer command_buffer, VkCommandBufferInheritanceInfo CONST_REFERENCE inheritance_info, VkCommandBufferUsageFlags usage);
    CVK_API void end_command_buffer(VkCommandBuffer command_buffer);

    // cmd
    CVK_API void cmd_bind_pipeline(VkCommandBuffer buffer, VkPipelineBindPoint bind_point, VkPipeline pipeline);
    CVK_API void cmd_bind_descriptor_sets(VkCommandBuffer buffer, VkPipelineBindPoint bind_point, VkPipelineLayout layout, std::vector<VkDescriptorSet> CONST_REFERENCE sets);
    CVK_API void cmd_bind_vertex_buffers(VkCommandBuffer buffer, std::vector<VkBuffer> CONST_REFERENCE vertex_buffers, std::vector<VkDeviceSize> CONST_REFERENCE offsets);
    CVK_API void cmd_bind_index_buffer(VkCommandBuffer buffer, VkIndexType type, VkBuffer index_buffer);
    CVK_API void cmd_set_viewport(VkCommandBuffer buffer, std::vector<VkViewport> CONST_REFERENCE viewports);
    CVK_API void cmd_set_scissor(VkCommandBuffer buffer, std::vector<VkRect2D> CONST_REFERENCE scissors);
    CVK_API void cmd_draw(VkCommandBuffer buffer, uint32_t vertex_count);
    CVK_API void cmd_draw_indexed(VkCommandBuffer buffer, uint32_t index_count, uint32_t first_index = 0);
    CVK_API void cmd_next_subpass(VkCommandBuffer buffer, VkSubpassContents contents);
    CVK_API void cmd_execute_command(VkCommandBuffer buffer, std::vector<VkCommandBuffer> CONST_REFERENCE buffers);
    CVK_API void cmd_copy_buffer(VkCommandBuffer buffer, VkBuffer src, VkBuffer dst, std::vector<VkBufferCopy> CONST_REFERENCE offset);
    CVK_API void cmd_copy_buffer_to_image(VkCommandBuffer buffer, VkBuffer src, VkImage dst, VkImageLayout dst_layout, std::vector<VkBufferImageCopy> CONST_REFERENCE offset);
    
    CVK_API void get_image_memory_barrier(VkImage image, VkImageSubresourceRange CONST_REFERENCE subresource, VkImageLayout src_layout, VkImageLayout dst_layout, VkImageMemoryBarrier& barrier);
    CVK_API void get_image_layout_access_mask(VkImageLayout layout, VkAccessFlags& access);
    CVK_API void cmd_set_pipeline_barrier(VkCommandBuffer buffer, VkPipelineStageFlags src, VkPipelineStageFlags dst, std::vector<VkBufferMemoryBarrier> CONST_REFERENCE buffer_barrier, std::vector<VkImageMemoryBarrier> CONST_REFERENCE image_barrier);

    // cmd renderpass
    CVK_API void get_default_begin_renderpass_info(VkRenderPass renderpass, VkFramebuffer framebuffer, std::vector<VkClearValue> CONST_REFERENCE clear_values, VkRect2D CONST_REFERENCE render_area, VkRenderPassBeginInfo& info);
    CVK_API void cmd_begin_renderpass(VkCommandBuffer buffer, VkRenderPassBeginInfo CONST_REFERENCE info, VkSubpassContents contents);
    CVK_API void cmd_end_renderpass(VkCommandBuffer buffer);
    // CVK_API void cmd_next_subpass(VkCommandBuffer buffer, VkSubpassContents contents);

    // queue
    CVK_API void get_queue(VkDevice device, uint32_t queue_index, VkQueue& queue);
    CVK_API VkResult queue_submit(VkPipelineStageFlags wait_stage, std::vector<VkSemaphore> CONST_REFERENCE wait, std::vector<VkSemaphore> CONST_REFERENCE signal, VkFence finish_fence, std::vector<VkCommandBuffer> CONST_REFERENCE buffers, VkQueue queue);
    
};