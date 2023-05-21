#pragma once

#include "cvk/device/device.h"
#include "cvk/device/device_feature.h"

namespace cvk
{

    class CVK_API DeviceFeatureRayTracing : public DeviceFeature<VkPhysicalDeviceRayTracingPipelineFeaturesKHR>
    {
    public:
        DeviceFeatureRayTracing();

    };
    
} // namespace cvk
