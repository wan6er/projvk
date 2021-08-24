#pragma once

#include "cvk/vk_header.h"

#include <vector>
#include <map>
#include <string>

namespace cvk
{

    class Device
    {
    public:
        Device(VkPhysicalDevice physical_device, const std::vector<std::string> &extensions_name, const VkPhysicalDeviceFeatures& features, uint32_t queue_flag);
        ~Device();

        operator VkDevice() const;

        auto get_queue_family_index(VkQueueFlagBits flag) const -> uint32_t;

    private:
        VkDevice _device;

        typedef std::map<VkQueueFlagBits, uint32_t> DeviceQueueFamilyIndexType;
        DeviceQueueFamilyIndexType _indices;
    };
};  