#include "cvk/swapchain.h"

#include "cvk/initialize/swapchain_initialize.h"

cvk::Swapchain::Swapchain(VkPhysicalDevice physical_device, VkDevice device, VkSurfaceKHR surface, VkPresentModeKHR present_mode) :
    _device(device)
{
    __cvk::create_swapchain(physical_device, _device, surface, present_mode, object());
}

cvk::Swapchain::operator ObjType() const
{
    return object();
}

void cvk::Swapchain::release()
{
    __cvk::destroy_swapchain(_device, object());
}