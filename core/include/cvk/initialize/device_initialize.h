#pragma once

#include "cvk/vk_header.h"

#include <string>
#include <vector>
#include <map>

namespace __cvk
{
    typedef std::map<VkQueueFlagBits, uint32_t> QueueIndexType;

    CVK_API void get_device_create_info(std::vector<VkDeviceQueueCreateInfo> CONST_REFERENCE queues, std::vector<char CONST_PTR> CONST_REFERENCE layers, std::vector<char CONST_PTR> CONST_REFERENCE exts, VkDeviceCreateInfo& create_info);
    CVK_API void get_device_queue_create_info(uint32_t index, VkDeviceQueueCreateInfo& info);
    // CVK_API void get_device_queue_family(VkPhysicalDevice device, VkQueueFlagBits queue_flag, uint32_t &index);

    CVK_API VkResult create_device(VkPhysicalDevice physical_device, VkDeviceCreateInfo CONST_REFERENCE create_info, VkDevice& device);
    CVK_API void destroy_device(VkDevice device);
    CVK_API VkResult wait_device_idle(VkDevice device);

    CVK_API void get_device_queue(VkDevice device, uint32_t index, VkQueue& queue);

    CVK_API void get_all_device_extensions(VkPhysicalDevice device, std::vector<std::string>& extensions);
    // CVK_API void get_all_device_queue_families(VkPhysicalDevice device, std::vector<VkQueueFamilyProperties>& queue_families);
    // CVK_API void get_all_device_queue_family_indices(VkPhysicalDevice device, QueueIndexType& queue_index);
    CVK_API void get_phydev_memory_properties(VkPhysicalDevice device, VkPhysicalDeviceMemoryProperties& properties);
    CVK_API void get_phydev_properties2(VkPhysicalDevice device, VkPhysicalDeviceProperties2& properties);

    // queue family
    CVK_API VkResult get_queue_family_properties(VkPhysicalDevice physical_device, std::vector<VkQueueFamilyProperties>& properties);
    CVK_API VkResult get_queue_family_index_present_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface, uint32_t index, VkBool32& support);
    CVK_API void get_queue_family_index(VkPhysicalDevice physical_device, VkQueueFlagBits queue_flag, uint32_t& index, uint32_t offset = 0);
    CVK_API void get_queue_family_index_present_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkQueueFlagBits queue_flag, uint32_t& index, uint32_t offset = 0);
};
