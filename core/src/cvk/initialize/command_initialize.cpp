#include "cvk/initialize/command_initialize.h"
#include "utils/vector_util.h"

namespace __cvk
{

CVK_API void get_default_command_pool_create_info(uint32_t queue_index, VkCommandPoolCreateInfo& create_info)
{
    create_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,        
        .queueFamilyIndex = queue_index,
    };
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
    VkCommandBufferBeginInfo cmd_buf_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = NULL,
        .flags = usage,
        .pInheritanceInfo = NULL,
    };
    return vkBeginCommandBuffer(command_buffer, &cmd_buf_info);
}

VkResult begin_command_buffer_secondary(VkCommandBuffer command_buffer, VkCommandBufferInheritanceInfo CONST_REFERENCE inheritance_info, VkCommandBufferUsageFlags usage)
{
    CVK_ASSERT(command_buffer != VK_NULL_HANDLE);
    VkCommandBufferBeginInfo cmd_buf_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = NULL,
        .flags = usage,
        .pInheritanceInfo = &inheritance_info,
    };
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

CVK_API void cmd_bind_vertex_buffers(VkCommandBuffer buffer, std::vector<VkBuffer> CONST_REFERENCE vertex_buffers)
{
    CVK_ASSERT(buffer != VK_NULL_HANDLE);
    CVK_ASSERT(vertex_buffers.size() > 0);
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(buffer, 0, static_cast<uint32_t>(vertex_buffers.size()), vertex_buffers.data(), &offset);
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

CVK_API void get_default_begin_renderpass_info(VkRenderPass renderpass, VkFramebuffer framebuffer, std::vector<VkClearValue> CONST_REFERENCE clear_values, VkRect2D CONST_REFERENCE render_area, VkRenderPassBeginInfo& info)
{
    info = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = renderpass,
        .framebuffer = framebuffer,
        .renderArea = render_area,
    };
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

};
