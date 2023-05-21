#pragma once

#include "cvk/device/device.h"
#include "cvk/device/device_feature.h"

namespace cvk
{

    class CVK_API DeviceFeatureAcceleration : public DeviceFeature<VkPhysicalDeviceAccelerationStructureFeaturesKHR>
    {
    public:
        DeviceFeatureAcceleration();

    };
    
} // namespace cvk
