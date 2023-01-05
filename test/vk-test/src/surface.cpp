#include "test_header.h"

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/memory.h"
#include "cvk/initialize/surface_initialize.h"
#include "utils/vector_util.h"

#ifdef WIN32
#include "win32/surface_win32.h"
#include "win32/win.h"
#endif

void test_queue_family(cvk::Device& device, cvk::Surface& surface)
{
    VkResult result = VK_SUCCESS;
    CHECK(result == VK_SUCCESS);
    uint32_t graphics_index = 0, present_index = 0;

    __cvk::get_queue_family_index(device.get_physical_device(), VK_QUEUE_GRAPHICS_BIT, graphics_index);
    CHECK(graphics_index != UINT32_MAX);

    __cvk::get_queue_family_index_present_support(device.get_physical_device(), surface, VK_QUEUE_GRAPHICS_BIT, present_index);
    CHECK(graphics_index != UINT32_MAX);
    CHECK(present_index != UINT32_MAX);
}

void test_surface(cvk::Device& device, cvk::Surface& surface)
{
    VkResult result = VK_SUCCESS;

    std::vector<VkSurfaceFormatKHR> formats;
    result = __cvk::get_surface_formats(device.get_physical_device(), surface, formats);
    CHECK(result == VK_SUCCESS);
    CHECK(formats.size() > 0);

    VkSurfaceCapabilitiesKHR capabilities;
    result = __cvk::get_surface_capabilities(device.get_physical_device(), surface, capabilities);
    CHECK(result == VK_SUCCESS);

    uint32_t width = 0, height = 0;
    VkCompositeAlphaFlagBitsKHR composite_alpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    uint32_t image_count = 2;
    VkSurfaceTransformFlagBitsKHR transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    result = __cvk::get_surface_info(device.get_physical_device(), surface, &width, &height, &image_count, &composite_alpha, &transform);
    CHECK(result == VK_SUCCESS);
    CHECK((width > 0 && height > 0));

    std::vector<VkPresentModeKHR> present_modes;
    result = __cvk::get_surface_present_modes(device.get_physical_device(), surface, present_modes);
    CHECK(result == VK_SUCCESS);
    CHECK(present_modes.size() > 0);

    std::vector<VkPresentModeKHR> best_mode;
    utils::vector_filter(present_modes, { VK_PRESENT_MODE_FIFO_KHR }, best_mode);
    CHECK(best_mode.size() > 0);
}

TEST_FUNC_BEGIN("surface")

    std::vector<std::string> instance_extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };
    std::vector<std::string> instance_layers = {
        "VK_LAYER_KHRONOS_validation"
    };
    std::vector<std::string> device_extensions = {
        // VK_EXT_DEBUG_MARKER_EXTENSION_NAME
    };

    cvk::Instance instance(instance_extensions, instance_layers);
    std::vector<VkPhysicalDevice>&& devices = instance.get_all_physical_device();
    VkPhysicalDeviceFeatures device_features = {};
    cvk::Device device(devices[0], device_extensions, {}, VK_QUEUE_GRAPHICS_BIT);

#ifdef WIN32
    Windows win("surface", 1024, 720);
    cvk::SurfaceWin32 surface(instance, win.instance(), win);
#else
#error unsupport platform
#endif

    test_queue_family(device, surface);
    test_surface(device, surface);

TEST_FUNC_END