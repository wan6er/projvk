#pragma once

#include "cvk/vk_header.h"

namespace cvk
{
    class CVK_API PhysicalDevice
    {
    public:
        explicit PhysicalDevice(VkPhysicalDevice physical_device);
        PhysicalDevice(const PhysicalDevice& physical_device) = default;

        auto get_memory_properties() const -> VkPhysicalDeviceMemoryProperties;
        auto get_physical_device() const -> VkPhysicalDevice;

        auto get_properties2() const -> VkPhysicalDeviceProperties2;
        auto get_raytracing_properties() const -> VkPhysicalDeviceRayTracingPipelinePropertiesKHR;

    private:
        VkPhysicalDevice _physical_device;
    };
};