#ifdef CVK_RAYTRACING

#include "test_header.h"

#include <vector>
#include <string>

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/buffer.h"

TEST_FUNC_BEGIN("acceleration struct")
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
    VkPhysicalDeviceFeatures device_features { };
    cvk::Device device(devices[0]);
    device.add_extensions(
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
        VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
        VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
    );
    device.add_device_features(
        cvk::DeviceFeatureAcceleration(),
        cvk::DeviceFeatureBufferAddress(),
        cvk::DeviceFeatureRayTracing()
    );
    CHECK(device.create(VK_QUEUE_GRAPHICS_BIT) == VK_SUCCESS);

    // uint32_t gindex = device.get_queue_family_index(VK_QUEUE_GRAPHICS_BIT);

    cvk::AccelerationStructureInputBuffer buffer(device);
    CHECK(buffer.create(device.get_memory_properties(), 10) == VK_SUCCESS);
    cvk::AccelerationStructureInputBufferWritable bufferw(device);
    CHECK(bufferw.create(device.get_memory_properties(), 10) == VK_SUCCESS);

TEST_FUNC_END

#endif