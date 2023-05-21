#include "cvk/device/device_feat_buffer_address.h"

namespace cvk
{

    DeviceFeatureBufferAddress::DeviceFeatureBufferAddress()
    {
        get_feature().sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_KHR;
        get_feature().bufferDeviceAddress = VK_TRUE;
    }
}