#include "test_header.h"

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/surface.h"
#include "cvk/command_pool.h"
#include "cvk/command_buffer.h"
#include "cvk/initialize/command_initialize.h"
#include "cvk/initialize/surface_initialize.h"


TEST_FUNC_BEGIN("command")
    std::vector<std::string> instance_extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        // VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };
    std::vector<std::string> instance_layers = {
        "VK_LAYER_KHRONOS_validation"
    };
    std::vector<std::string> device_extensions = {
        // VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        // VK_EXT_DEBUG_MARKER_EXTENSION_NAME
    };

    cvk::Instance instance(instance_extensions, instance_layers);
    std::vector<VkPhysicalDevice>&& devices = instance.get_all_physical_device();
    VkPhysicalDeviceFeatures device_features = {};
    cvk::Device device(devices[0], device_extensions, device_features, VK_QUEUE_GRAPHICS_BIT);

    VkResult result = VK_SUCCESS;
    CHECK(result == VK_SUCCESS);
    
    uint32_t graphics_index = 0;
    __cvk::get_queue_family_index(device.get_physical_device(), VK_QUEUE_GRAPHICS_BIT, graphics_index);

    {
        cvk::CommandPool pool(device, graphics_index);
        CHECK(pool.create() == VK_SUCCESS);
        cvk::CommandBuffer cmd_buf(device, pool);
        CHECK(cmd_buf.create() == VK_SUCCESS);
        cvk::CommandBufferSecondary cmd_buf2(device, pool);
        CHECK(cmd_buf2.create() == VK_SUCCESS);
        
    }

    {
        cvk::CommandPool tpool(device, graphics_index);
        CHECK(tpool.transient().create() == VK_SUCCESS);
        cvk::CommandBuffer tcmd_buf(device, tpool);
        CHECK(tcmd_buf.create() == VK_SUCCESS);
        cvk::CommandBufferSecondary tcmd_buf2(device, tpool);
        // CHECK(tcmd_buf2.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) == VK_SUCCESS);
        CHECK(tcmd_buf2.create() == VK_SUCCESS);
    }

TEST_FUNC_END