#pragma once

#include "cvk/vk_header.h"

namespace cvk
{
    class CVK_API PhysicalDevice
    {
    public:
        explicit PhysicalDevice(VkPhysicalDevice physical_device);
        PhysicalDevice(const PhysicalDevice& physical_device) = default;

        auto get_memory_properties() const -> const VkPhysicalDeviceMemoryProperties&;
        auto get_physical_device() const -> VkPhysicalDevice;

    private:
        VkPhysicalDevice _physical_device;
        VkPhysicalDeviceMemoryProperties _memory_properties;
    };
};