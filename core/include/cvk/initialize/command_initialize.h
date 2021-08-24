#pragma once

#include "cvk/vk_header.h"

#include <vector>

namespace __cvk
{
    VkResult create_command_pool(VkDevice device, uint32_t queue_index, VkCommandPoolCreateFlags flags, VkCommandPool& command_pool);
    VkResult create_command_buffers(VkDevice device, VkCommandPool command_pool, VkCommandBufferLevel level, uint32_t size, std::vector<VkCommandBuffer>& command_buffers);
    VkResult create_semaphore(VkDevice device, VkSemaphore& semaphore);
    VkResult create_fence(VkDevice device, bool signal, VkFence& fence);
};