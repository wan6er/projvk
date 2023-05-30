#ifdef CVK_RAYTRACING

#include "test_header.h"

#include <vector>
#include <string>

#include "cvk/instance.h"
#include "cvk/device.h"

#include "cvk/pipeline_raytracing.h"
#include "cvk/descriptor.h"
#include "cvk/shader.h"
#include "utils/file.h"

TEST_FUNC_BEGIN("raytracing pipeline")
    std::vector<std::string> instance_extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#ifdef WIN32
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
    };
    std::vector<std::string> instance_layers = {
        "VK_LAYER_KHRONOS_validation"
    };

    cvk::Instance instance(instance_extensions, instance_layers);
    std::vector<VkPhysicalDevice>&& devices = instance.get_all_physical_device();
    cvk::Device device(devices[0]);
    device.add_extensions(
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
        VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
        VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
        VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    );
    device.add_device_features(
        cvk::DeviceFeatureAcceleration(),
        cvk::DeviceFeatureBufferAddress(),
        cvk::DeviceFeatureRayTracing()
    );
    CHECK(device.create(VK_QUEUE_GRAPHICS_BIT) == VK_SUCCESS);

    uint32_t grapindex;
    __cvk::get_queue_family_index(device.get_physical_device(), VK_QUEUE_GRAPHICS_BIT, grapindex);


    cvk::Descriptor descriptor(device);
    descriptor.add_layout()
        .set(0, VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, VK_SHADER_STAGE_RAYGEN_BIT_KHR, 1)
        .set(1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, VK_SHADER_STAGE_RAYGEN_BIT_KHR, 1)
        .set(2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_RAYGEN_BIT_KHR, 1);
    CHECK(descriptor.create() == VK_SUCCESS);

    cvk::Shader rgen(device, utils::load_file("shader/raygen.rgen.spv"));
    CHECK(rgen.create() == VK_SUCCESS);
    cvk::Shader rmiss(device, utils::load_file("shader/miss.rmiss.spv"));
    CHECK(rmiss.create() == VK_SUCCESS);
    cvk::Shader rchit(device, utils::load_file("shader/closesthit.rchit.spv"));
    CHECK(rchit.create() == VK_SUCCESS);

    cvk::PipelineLayout layout(device);
    layout.attaches(descriptor[0].get_layout());
    CHECK(layout.create() == VK_SUCCESS);
    cvk::RayTracingPipeline pipeline(device);
    pipeline.add_stage(VK_SHADER_STAGE_RAYGEN_BIT_KHR, rgen);
    pipeline.add_stage(VK_SHADER_STAGE_MISS_BIT_KHR, rmiss);
    pipeline.add_stage(VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR, rchit);
    CHECK(pipeline.create(layout) == VK_SUCCESS);

TEST_FUNC_END

#endif