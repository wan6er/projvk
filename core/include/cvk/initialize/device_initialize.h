#pragma once

#include "cvk/vk_header.h"

#include <string>
#include <vector>
#include <map>

namespace __cvk
{
    typedef std::map<VkQueueFlagBits, uint32_t> QueueIndexType;

    VkResult create_device(VkPhysicalDevice physical_device, const __cvk::QueueIndexType& indices, const std::vector<std::string>& extensions_name, const VkPhysicalDeviceFeatures& features, uint32_t queue_flag, VkDevice& device);
    void destroy_device(VkDevice device);

    void get_device_queue(VkDevice device, uint32_t index, VkQueue& queue);

    void get_all_device_extensions(VkPhysicalDevice device, std::vector<std::string>& extensions);
    void get_all_device_queue_families(VkPhysicalDevice device, std::vector<VkQueueFamilyProperties>& queue_families);
    void get_all_device_queue_family_indices(VkPhysicalDevice device, QueueIndexType& queue_index);
    void get_device_memory_properties(VkPhysicalDevice device, VkPhysicalDeviceMemoryProperties& properties);
};
