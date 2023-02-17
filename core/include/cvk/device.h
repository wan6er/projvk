#pragma once

#include "utils/base_obj.h"
#include "cvk/vk_header.h"
#include "cvk/physical_device.h"

#include <vector>
#include <map>
#include <string>

namespace cvk
{

    class CVK_API Device : protected utils::BaseObj<VkDevice>, public PhysicalDevice
    {
    public:
        Device(VkPhysicalDevice physical_device, const std::vector<std::string> &extensions_name, const VkPhysicalDeviceFeatures& features, uint32_t queue_flag);
        Device(const Device& device) = default;
        ~Device();

        operator VkDevice() const;

        auto get_queue_family_index(VkQueueFlagBits flag) const -> uint32_t;
        auto get_queue(VkQueueFlagBits flag) const -> VkQueue;

    protected:
        void release();

    private:
        typedef std::map<VkQueueFlagBits, uint32_t> DeviceQueueFamilyIndexType;
        DeviceQueueFamilyIndexType _indices;
    };
};  