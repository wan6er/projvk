#pragma once

#include "cvk/device/device.h"
#include "cvk/device/device_feature.h"

namespace cvk
{

    class CVK_API DeviceFeatureMaintenance4 : public DeviceFeature<VkPhysicalDeviceMaintenance4Features>
    {
    public:
        DeviceFeatureMaintenance4();

    };
    
} // namespace cvk
