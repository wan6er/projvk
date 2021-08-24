#include "cvk/initialize/command_initialize.h"

VkResult __cvk::create_command_pool(VkDevice device, uint32_t queue_index, VkCommandPoolCreateFlags flags, VkCommandPool& command_pool)
{
    VkCommandPoolCreateInfo command_pool_create_info = {};
    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.queueFamilyIndex = queue_index;
    command_pool_create_info.flags = flags;
    return vkCreateCommandPool(device, &command_pool_create_info, nullptr, &command_pool);
}

VkResult __cvk::create_command_buffers(VkDevice device, VkCommandPool command_pool, VkCommandBufferLevel level, uint32_t size, std::vector<VkCommandBuffer>& command_buffers)
{
	command_buffers.resize(size);
    VkCommandBufferAllocateInfo command_buffer_allocate_info{};
    command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.commandPool = command_pool;
    command_buffer_allocate_info.level = level;
    command_buffer_allocate_info.commandBufferCount = size;
    return vkAllocateCommandBuffers(device, &command_buffer_allocate_info, command_buffers.data());
}

VkResult __cvk::create_semaphore(VkDevice device, VkSemaphore& semaphore)
{
    VkSemaphoreCreateInfo semaphore_create_info{};
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    return vkCreateSemaphore(device, &semaphore_create_info, nullptr, &semaphore);
}

VkResult __cvk::create_fence(VkDevice device, bool signal, VkFence& fence)
{
    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = signal ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
    return vkCreateFence(device, &fenceCreateInfo, nullptr, &fence);
}