#pragma once

#include "cvk/vk_header.h"
// #include "cvk/initialize/device_initialize.h"

#include <vector>

namespace __cvk
{
    VkResult create_swapchain(VkPhysicalDevice physical_device, VkDevice device, VkSurfaceKHR surface, VkPresentModeKHR present_mode, VkSwapchainKHR& swapchain, VkSwapchainKHR old_swapchain = nullptr);
    void destroy_swapchain(VkDevice device, VkSwapchainKHR swapchain);

    void check_queue_family_present_available(VkPhysicalDevice device, VkSurfaceKHR surface, uint32_t index, VkBool32& available);

    void get_all_device_formats(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR>& formats);
    void get_all_present_modes(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR>& present_modes);
    void get_all_swapchain_image(VkDevice device, VkSwapchainKHR swapchain, std::vector<VkImage>& images);
    void get_device_format(VkPhysicalDevice device, VkSurfaceKHR surface, VkSurfaceFormatKHR& surface_format, VkFormat = VK_FORMAT_B8G8R8A8_UNORM);
};