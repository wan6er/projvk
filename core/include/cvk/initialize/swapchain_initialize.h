#pragma once

#include "cvk/vk_header.h"
// #include "cvk/initialize/device_initialize.h"

#include <vector>

namespace __cvk
{
    CVK_API VkResult create_swapchain(VkPhysicalDevice physical_device, VkDevice device, VkSurfaceKHR surface, VkSwapchainKHR& swapchain, 
        std::vector<VkPresentModeKHR> present_modes,
        VkSurfaceFormatKHR surface_format,
        uint32_t width = 0, uint32_t height = 0,
        uint32_t image_count = 0,
        uint32_t image_array_layers = 1,
        VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        VkSurfaceTransformFlagBitsKHR transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        VkCompositeAlphaFlagBitsKHR composite_alpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VkSharingMode sharing_mode = VK_SHARING_MODE_EXCLUSIVE,
        uint32_t queue_family_count = 0,
        VkSwapchainKHR old_swapchain = nullptr
    );    

    CVK_API VkResult get_default_swapchain_create_info(VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkPresentModeKHR present_mode, VkSurfaceFormatKHR surface_format, VkSwapchainCreateInfoKHR &create_info);
    CVK_API void get_swapchain_image_view_create_info(const VkSwapchainCreateInfoKHR &swapchain_create_info, VkImageViewCreateInfo& view_create_info);
    CVK_API VkResult create_swapchain_by_info(VkDevice device, const VkSwapchainCreateInfoKHR &create_info, VkSwapchainKHR &swapchain);
    CVK_API void destroy_swapchain(VkDevice device, VkSwapchainKHR swapchain);
    CVK_API VkResult get_swapchain_images(VkDevice device, VkSwapchainKHR swapchain, std::vector<VkImage>& images);

    // present
    CVK_API VkResult swapchain_acquire_next_image(VkDevice device, VkSwapchainKHR swapchain, uint32_t& next_index, VkSemaphore wait_semaphore, VkFence wait_fence);
    CVK_API VkResult swapchain_present(VkQueue queue, VkSwapchainKHR CONST_REFERENCE swapchain, std::vector<VkSemaphore> CONST_REFERENCE wait, uint32_t& next_index);

    // CVK_API void check_queue_family_present_available(VkPhysicalDevice device, VkSurfaceKHR surface, uint32_t index, VkBool32& available);

    // CVK_API void get_all_device_formats(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR>& formats);
    // CVK_API void get_all_present_modes(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR>& present_modes);
    // CVK_API void get_device_format(VkPhysicalDevice device, VkSurfaceKHR surface, VkSurfaceFormatKHR& surface_format, VkFormat = VK_FORMAT_B8G8R8A8_UNORM);
};