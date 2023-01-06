#pragma once

#include "utils/base_obj.h"
#include "utils/base_multi_attach.h"
#include "cvk/vk_header.h"

namespace cvk
{
    class CVK_API Swapchain : 
        protected utils::BaseObj<VkSwapchainKHR>,
        protected utils::BaseMultipleAttaches<VkImage>
    {
    public:
        Swapchain(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR> CONST_REFERENCE present_modes, VkSurfaceFormatKHR surface_format);
        Swapchain(VkDevice device, VkSwapchainCreateInfoKHR CONST_REFERENCE create_info);
        virtual ~Swapchain();

        VkSwapchainCreateInfoKHR& info();
        VkResult create();
        auto get_images() const -> std::vector<VkImage> CONST_REFERENCE;

        uint32_t acquire(VkSemaphore signal_semaphore, VkFence signal_fence = VK_NULL_HANDLE);
        VkResult present(VkQueue queue, std::vector<VkSemaphore> CONST_REFERENCE wait);

        operator ObjType CONST_REFERENCE () const;

    protected:
        void init_info(VkPhysicalDevice physical_device, VkSurfaceKHR surface, const std::vector<VkPresentModeKHR>& present_modes, VkSurfaceFormatKHR surface_format);

        void release();
    private:
        VkSwapchainCreateInfoKHR _create_info;
        VkDevice _device = VK_NULL_HANDLE;
        uint32_t _cur_present_index = UINT32_MAX;
    };
};