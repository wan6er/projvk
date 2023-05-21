#pragma once

#include "cvk/vk_header.h"

#include <vector>

namespace __cvk
{
    CVK_API void destroy_surface(VkInstance instance, VkSurfaceKHR surface);
    CVK_API VkResult get_surface_formats(VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR>& formats);
    CVK_API VkResult get_surface_capabilities(VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR& capabilities);
    CVK_API VkResult get_surface_present_modes(VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR>& present_modes);
    // CVK_API VkResult get_surface_size(VkPhysicalDevice physical_device, VkSurfaceKHR surface, uint32_t& width, uint32_t& height);
    CVK_API VkResult get_surface_info(VkPhysicalDevice physical_device, VkSurfaceKHR surface, uint32_t* width, uint32_t* height, uint32_t* image_count, VkCompositeAlphaFlagBitsKHR* composite_alpha = nullptr, VkSurfaceTransformFlagBitsKHR* transform = nullptr);

}; 