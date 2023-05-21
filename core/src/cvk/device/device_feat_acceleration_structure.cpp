#include "cvk/device/device_feat_acceleration_structure.h"

namespace cvk
{

    DeviceFeatureAcceleration::DeviceFeatureAcceleration()
    {
        get_feature().sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
        get_feature().accelerationStructure = VK_TRUE;
    }
}