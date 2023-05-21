#pragma once

#include "cvk/device/device.h"
#include "cvk/device/device_feature.h"

namespace cvk
{

    class CVK_API DeviceFeatureBufferAddress : public DeviceFeature<VkPhysicalDeviceBufferDeviceAddressFeaturesKHR>
    {
    public:
        DeviceFeatureBufferAddress();

    };
    
} // namespace cvk
