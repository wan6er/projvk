#include "test_header.h"

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/initialize/instance_initialize.h"

TEST_FUNC_BEGIN("instance")
    std::vector<std::string> instance_extensions = {
    };
    std::vector<std::string> instance_layers = {
        // "VK_LAYER_KHRONOS_validation"
    };
    std::vector<std::string> device_extensions = {
        // VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        // VK_EXT_DEBUG_MARKER_EXTENSION_NAME
    };

    cvk::Instance instance(instance_extensions, instance_layers);
    CHECK(static_cast<VkInstance>(instance) != nullptr);
    
TEST_FUNC_END


TEST_FUNC_BEGIN("device")

    std::vector<std::string> instance_extensions = {
    };
    std::vector<std::string> instance_layers = {
    };
    std::vector<std::string> device_extensions = {
    };

    {
        VkInstance instance;
        VkResult result = __cvk::create_instance(instance_extensions, instance_layers, instance);
        CHECK(result == VK_SUCCESS);
        __cvk::destroy_instance(instance);
    }

    cvk::Instance instance(instance_extensions, instance_layers);
    CHECK(static_cast<VkInstance>(instance) != nullptr);
    
    std::vector<VkPhysicalDevice>&& devices = instance.get_all_physical_device();
    CHECK(devices.size() > 0);

    VkPhysicalDeviceFeatures device_features = {};
    cvk::Device device(devices[0]);

    device.add_extensions(
#ifdef CVK_RAYTRACING
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#endif
    );
    device.add_device_features(
#ifdef CVK_RAYTRACING
        cvk::DeviceFeatureAcceleration()
#endif
    );
    device.create();

    CHECK(static_cast<VkDevice>(device) != nullptr);

TEST_FUNC_END