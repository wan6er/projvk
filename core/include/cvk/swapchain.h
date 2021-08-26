#pragma once

#include "utils/base_obj.h"
#include "cvk/vk_header.h"

namespace cvk
{
    class Swapchain : public utils::BaseObj<VkSwapchainKHR>
    {
    public:
        Swapchain(VkPhysicalDevice physical_device, VkDevice device, VkSurfaceKHR surface, VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR);

        operator ObjType() const;

    protected:
        virtual void release();
    private:
        VkDevice _device;
    };
};