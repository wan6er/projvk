#include "cvk/device/device_feat_raytracing.h"

namespace cvk
{

    DeviceFeatureRayTracing::DeviceFeatureRayTracing()
    {
        get_feature().sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
        get_feature().rayTracingPipeline = VK_TRUE;
    }
}