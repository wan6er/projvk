#include "cvk/device/device_feat_maintenance4.h"

namespace cvk
{

    DeviceFeatureMaintenance4::DeviceFeatureMaintenance4()
    {
        get_feature().sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_FEATURES_KHR;
        get_feature().maintenance4 = VK_TRUE;
    }
}