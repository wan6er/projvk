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

    // queue family
    CVK_API VkResult get_queue_family_properties(VkPhysicalDevice physical_device, std::vector<VkQueueFamilyProperties>& properties);
    CVK_API VkResult get_queue_family_index_present_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface, uint32_t index, VkBool32& support);
    // CVK_API void get_queue_family_index(VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkQueueFamilyProperties> CONST_REFERENCE properties, VkQueueFlagBits queue_flag, uint32_t& index, uint32_t& present_index);
    // CVK_API void get_queue_family_index(std::vector<VkQueueFamilyProperties> CONST_REFERENCE properties, VkQueueFlagBits queue_flag, uint32_t& index, uint32_t offset);
    CVK_API void get_queue_family_index(VkPhysicalDevice physical_device, VkQueueFlagBits queue_flag, uint32_t& index, uint32_t offset = 0);
    CVK_API void get_queue_family_index_present_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkQueueFlagBits queue_flag, uint32_t& index, uint32_t offset = 0);
}; 