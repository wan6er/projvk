#include "cvk/initialize/sync_initialize.h"

namespace __cvk
{


VkResult create_fence(VkDevice device, VkFence& fence, bool signaled) 
{
    VkFenceCreateInfo fence_info = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO
    };
    if (signaled) {
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }
    return vkCreateFence(device, &fence_info, nullptr, &fence);
}

void destroy_fence(VkDevice device, VkFence fence)
{
    vkDestroyFence(device, fence, nullptr);
}

CVK_API VkResult wait_fences(VkDevice device, std::vector<VkFence> CONST_REFERENCE fences, uint32_t timeout)
{
    return vkWaitForFences(device, static_cast<uint32_t>(fences.size()), fences.data(), VK_TRUE, timeout);
}

VkResult create_semaphore(VkDevice device, VkSemaphore& semaphore, bool signaled)
{
    VkSemaphoreCreateInfo semaphore_info = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };
    if (signaled) {
        semaphore_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }
    return vkCreateSemaphore(device, &semaphore_info, nullptr, &semaphore);
}

void destroy_semaphore(VkDevice device, VkSemaphore semaphore)
{
    vkDestroySemaphore(device, semaphore, nullptr);
}

// CVK_API VkResult wait_semaphores(VkDevice device, std::vector<VkSemaphore> CONST_REFERENCE semaphores, uint32_t timeout)
// {
//     return vkWaitSemaphores(device, static_cast<uint32_t>(semaphores.size()), semaphores.data(), VK_TRUE, timeout);
// }

};