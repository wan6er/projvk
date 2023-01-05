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
        Swapchain(VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR> CONST_REFERENCE present_modes, VkSurfaceFormatKHR surface_format);
        Swapchain(VkSwapchainCreateInfoKHR CONST_REFERENCE create_info);
        virtual ~Swapchain();

        VkSwapchainCreateInfoKHR& info();
        VkResult create(VkDevice device);
        auto get_images() const -> std::vector<VkImage> CONST_REFERENCE;

        operator ObjType CONST_REFERENCE () const;

    protected:
        void init_info(VkPhysicalDevice physical_device, VkSurfaceKHR surface, const std::vector<VkPresentModeKHR>& present_modes, VkSurfaceFormatKHR surface_format);

        void release();
    private:
        VkSwapchainCreateInfoKHR _create_info;
        VkDevice _device = VK_NULL_HANDLE;
    };
};