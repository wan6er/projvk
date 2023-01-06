#pragma once

#include "cvk/vk_header.h"

namespace __cvk
{
    CVK_API VkResult create_fence(VkDevice device, VkFence& fence, bool signaled = false);
    CVK_API void destroy_fence(VkDevice device, VkFence fence);
    CVK_API VkResult wait_fences(VkDevice device, std::vector<VkFence> CONST_REFERENCE fences, uint32_t timeout);
    CVK_API VkResult reset_fences(VkDevice device, std::vector<VkFence> CONST_REFERENCE fences);

    CVK_API VkResult create_semaphore(VkDevice device, VkSemaphore& semaphore, bool signaled = false);
    CVK_API void destroy_semaphore(VkDevice device, VkSemaphore semaphore);
    CVK_API VkResult wait_semaphores(VkDevice device, std::vector<VkSemaphore> CONST_REFERENCE semaphores, uint32_t timeout);

}; 