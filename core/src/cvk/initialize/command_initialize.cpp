#include "cvk/initialize/command_initialize.h"
#include "utils/vector_util.h"

namespace __cvk
{

CVK_API void get_default_command_pool_create_info(uint32_t queue_index, VkCommandPoolCreateInfo& create_info)
{
    create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    create_info.queueFamilyIndex = queue_index;
}

VkResult create_command_pool(VkDevice device, VkCommandPoolCreateInfo CONST_REFERENCE create_info, VkCommandPool& command_pool)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    return vkCreateCommandPool(device, &create_info, nullptr, &command_pool);
}
void destroy_command_pool(VkDevice device, VkCommandPool command_pool)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(command_pool != VK_NULL_HANDLE);
    vkDestroyCommandPool(device, command_pool, nullptr);
}

CVK_API void get_default_command_buffer_inheritance_allocate_info(VkRenderPass renderpass, VkFramebuffer framebuffer, uint32_t subpass, VkCommandBufferInheritanceInfo& info)
{
    info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    info.renderPass = renderpass;
    info.framebuffer = framebuffer;
    info.subpass = subpass; 
}

CVK_API void get_default_command_buffers_allocate_info(VkCommandPool pool, VkCommandBufferLevel level, uint32_t size, VkCommandBufferAllocateInfo& info)
{
    info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.commandPool = pool;
    info.level = level;
    info.commandBufferCount = size;
}

CVK_API VkResult append_alloc_command_buffers(VkDevice device, VkCommandBufferAllocateInfo CONST_REFERENCE info, std::vector<VkCommandBuffer>& buffers)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    size_t cur = buffers.size();
    buffers.resize(cur + info.commandBufferCount);
    return vkAllocateCommandBuffers(device, &info, &buffers[cur]);
}

CVK_API VkResult alloc_command_buffers(VkDevice device, VkCommandBufferAllocateInfo CONST_REFERENCE info, std::vector<VkCommandBuffer>& buffers)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(info.commandBufferCount == buffers.size());
    return vkAllocateCommandBuffers(device, &info, buffers.data());
}

CVK_API VkResult alloc_command_buffer(VkDevice device, VkCommandBufferAllocateInfo CONST_REFERENCE info, VkCommandBuffer& buffer)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    return vkAllocateCommandBuffers(device, &info, &buffer);
}

CVK_API void free_command_buffers(VkDevice device, VkCommandPool command_pool, std::vector<VkCommandBuffer> CONST_REFERENCE buffers)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(command_pool != VK_NULL_HANDLE);
    vkFreeCommandBuffers(device, command_pool, static_cast<uint32_t>(buffers.size()), buffers.data());
}

VkResult begin_command_buffer(VkCommandBuffer command_buffer, VkCommandBufferUsageFlags usage)
{
    CVK_ASSERT(command_buffer != VK_NULL_HANDLE);
    VkCommandBufferBeginInfo cmd_buf_info = {};
    cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_info.flags = usage;
    cmd_buf_info.pInheritanceInfo = NULL;
    return vkBeginCommandBuffer(command_buffer, &cmd_buf_info);
}

VkResult begin_command_buffer_secondary(VkCommandBuffer command_buffer, VkCommandBufferInheritanceInfo CONST_REFERENCE inheritance_info, VkCommandBufferUsageFlags usage)
{
    CVK_ASSERT(command_buffer != VK_NULL_HANDLE);
    VkCommandBufferBeginInfo cmd_buf_info = {};
    cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_info.flags = usage;
    cmd_buf_info.pInheritanceInfo = &inheritance_info;
    return vkBeginCommandBuffer(command_buffer, &cmd_buf_info);
}

void end_command_buffer(VkCommandBuffer command_buffer)
{
    CVK_ASSERT(command_buffer != VK_NULL_HANDLE);
    vkEndCommandBuffer(command_buffer);
}

CVK_API void cmd_bind_pipeline(VkCommandBuffer buffer, VkPipelineBindPoint bind_point, VkPipeline pipeline)
{
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
    CVK_ASSERT(pipeline != VK_NULL_HANDLE);
    vkCmdBindPipeline(buffer, bind_point, pipeline);
}

CVK_API void cmd_bind_descriptor_sets(VkCommandBuffer buffer, VkPipelineBindPoint bind_point, VkPipelineLayout layout, std::vector<VkDescriptorSet> CONST_REFERENCE sets)
{
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
    CVK_ASSERT(layout != VK_NULL_HANDLE);
    CVK_ASSERT(sets.size() > 0);
    vkCmdBindDescriptorSets(buffer, bind_point, layout, 0, static_cast<uint32_t>(sets.size()), sets.data(), 0, nullptr);
}

CVK_API void cmd_bind_vertex_buffers(VkCommandBuffer buffer, std::vector<VkBuffer> CONST_REFERENCE vertex_buffers, std::vector<VkDeviceSize> CONST_REFERENCE offsets)
{
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
    CVK_ASSERT(vertex_buffers.size() > 0 && vertex_buffers.size() == offsets.size());
    vkCmdBindVertexBuffers(buffer, 0, static_cast<uint32_t>(vertex_buffers.size()), vertex_buffers.data(), offsets.data());
}

CVK_API void cmd_bind_index_buffer(VkCommandBuffer buffer, VkIndexType type, VkBuffer index_buffer)
{
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
    CVK_ASSERT(index_buffer != VK_NULL_HANDLE);
    vkCmdBindIndexBuffer(buffer, index_buffer, 0, type);
}

CVK_API void cmd_set_viewport(VkCommandBuffer buffer, std::vector<VkViewport> CONST_REFERENCE viewports)
{
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
    CVK_ASSERT(viewports.size() > 0);
    vkCmdSetViewport(buffer, 0, static_cast<uint32_t>(viewports.size()), viewports.data());
}

CVK_API void cmd_set_scissor(VkCommandBuffer buffer, std::vector<VkRect2D> CONST_REFERENCE scissors)
{
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
    CVK_ASSERT(scissors.size() > 0);
    vkCmdSetScissor(buffer, 0, static_cast<uint32_t>(scissors.size()), scissors.data());
}

CVK_API void cmd_draw(VkCommandBuffer buffer, uint32_t vertex_count)
{
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
    CVK_ASSERT(vertex_count > 0);
    vkCmdDraw(buffer, vertex_count, 1, 0, 0);
}

CVK_API void cmd_draw_indexed(VkCommandBuffer buffer, uint32_t index_count, uint32_t first_index)
{
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
    CVK_ASSERT(index_count > 0);
    vkCmdDrawIndexed(buffer, index_count, 1, first_index, 0, 0);
}

CVK_API void cmd_next_subpass(VkCommandBuffer buffer, VkSubpassContents contents)
{
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
    vkCmdNextSubpass(buffer, contents);
}

CVK_API void cmd_execute_command(VkCommandBuffer buffer, std::vector<VkCommandBuffer> CONST_REFERENCE buffers)
{
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
    CVK_ASSERT(buffers.size() > 0);
    vkCmdExecuteCommands(buffer, static_cast<uint32_t>(buffers.size()), buffers.data());
}

CVK_API void cmd_copy_buffer(VkCommandBuffer buffer, VkBuffer src, VkBuffer dst, std::vector<VkBufferCopy> CONST_REFERENCE offset)
{
    vkCmdCopyBuffer(buffer, src, dst, static_cast<uint32_t>(offset.size()), offset.size() == 0 ? nullptr : offset.data());
}

CVK_API void get_image_memory_barrier(VkImage image, VkImageSubresourceRange CONST_REFERENCE subresource, VkImageLayout src_layout, VkImageLayout dst_layout, VkImageMemoryBarrier& barrier)
{
    barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange = subresource;
    barrier.oldLayout = src_layout;
    barrier.newLayout = dst_layout;
    get_image_layout_access_mask(src_layout, barrier.srcAccessMask);
    get_image_layout_access_mask(dst_layout, barrier.dstAccessMask);
}

CVK_API void get_image_layout_access_mask(VkImageLayout layout, VkAccessFlags& access)
{
    switch (layout)
    {
    case VK_IMAGE_LAYOUT_UNDEFINED:
        access = 0;
        break;
    case VK_IMAGE_LAYOUT_PREINITIALIZED:
        access = VK_ACCESS_HOST_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        access = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        access = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        access = VK_ACCESS_TRANSFER_READ_BIT;
        break;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        access = VK_ACCESS_TRANSFER_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        access = VK_ACCESS_SHADER_READ_BIT;
        break;
    default:
        break;
    }
}

CVK_API void cmd_set_pipeline_barrier(VkCommandBuffer buffer, VkPipelineStageFlags src, VkPipelineStageFlags dst, std::vector<VkBufferMemoryBarrier> CONST_REFERENCE buffer_barrier, std::vector<VkImageMemoryBarrier> CONST_REFERENCE image_barrier)
{
    vkCmdPipelineBarrier(buffer, src, dst, 0, 0, nullptr, 
        static_cast<uint32_t>(buffer_barrier.size()), buffer_barrier.data(),
        static_cast<uint32_t>(image_barrier.size()), image_barrier.data());
}

CVK_API void cmd_copy_buffer_to_image(VkCommandBuffer buffer, VkBuffer src, VkImage dst, VkImageLayout dst_layout, std::vector<VkBufferImageCopy> CONST_REFERENCE offset)
{
    vkCmdCopyBufferToImage(buffer, src, dst, dst_layout, static_cast<uint32_t>(offset.size()), offset.data());
}

CVK_API void get_default_begin_renderpass_info(VkRenderPass renderpass, VkFramebuffer framebuffer, std::vector<VkClearValue> CONST_REFERENCE clear_values, VkRect2D CONST_REFERENCE render_area, VkRenderPassBeginInfo& info)
{
    info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.renderPass = renderpass;
    info.framebuffer = framebuffer;
    info.renderArea = render_area;
    utils::vector_fill_info(clear_values, info.clearValueCount, info.pClearValues);
}

CVK_API void cmd_begin_renderpass(VkCommandBuffer buffer, VkRenderPassBeginInfo CONST_REFERENCE info, VkSubpassContents contents)
{
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
    vkCmdBeginRenderPass(buffer, &info, contents);
}

CVK_API void cmd_end_renderpass(VkCommandBuffer buffer)
{
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
    vkCmdEndRenderPass(buffer);
}

CVK_API void get_queue(VkDevice device, uint32_t queue_index, VkQueue& queue)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    vkGetDeviceQueue(device, queue_index, 0, &queue);
}

CVK_API VkResult queue_submit(VkPipelineStageFlags wait_stage, std::vector<VkSemaphore> CONST_REFERENCE wait, std::vector<VkSemaphore> CONST_REFERENCE signal, VkFence finish_fence, std::vector<VkCommandBuffer> CONST_REFERENCE buffers, VkQueue queue)
{
    CVK_ASSERT(queue != VK_NULL_HANDLE);
    CVK_ASSERT(buffers.size() > 0);
    VkSubmitInfo submit_info = {};
    submit_info.pNext = NULL;
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pWaitDstStageMask = &wait_stage;
    utils::vector_fill_info(wait, submit_info.waitSemaphoreCount, submit_info.pWaitSemaphores);
    utils::vector_fill_info(signal, submit_info.signalSemaphoreCount, submit_info.pSignalSemaphores);
    utils::vector_fill_info(buffers, submit_info.commandBufferCount, submit_info.pCommandBuffers);
    return vkQueueSubmit(queue, 1, &submit_info, finish_fence);
}

};
