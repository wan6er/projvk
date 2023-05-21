#include "test_header.h"

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/memory.h"
#include "cvk/buffer.h"
#include "cvk/image/image_view.h"
#include "cvk/image.h"
#include "cvk/initialize/surface_initialize.h"
#include "cvk/initialize/image_initialize.h"
#include "cvk/initialize/memory_initialize.h"


#ifdef WIN32
    #include "win32/surface_win32.h"
    #include "win32/win.h"
#elif linux
    #include "linux/win_xcb.h"
    #include "linux/surface_xcb.h"
#endif

TEST_FUNC_BEGIN("image")

    std::vector<std::string> instance_extensions = {
#ifdef WIN32
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif linux
        VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#endif
        VK_KHR_SURFACE_EXTENSION_NAME,
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
    cvk::Device device(devices[0]);
    device.add_extensions(
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    );
    device.create(VK_QUEUE_GRAPHICS_BIT);

    uint32_t width = 1024, height = 720;

#ifdef WIN32
    Windows win;
    win.create("image", width, height);
    win.show();
    cvk::SurfaceWin32 surface(instance, win.instance(), win);
#elif linux
    XCBWindow win;
    win.create("image", width, height);
    win.show();
    cvk::SurfaceXCB surface(instance, win.get_connection(), win.get_window());
#else
#error unsupport platform
#endif

    auto test_color_image = [&] {
        cvk::basic::Image image(device);
        image.setup(VK_FORMAT_R8G8B8A8_UNORM, {1000, 700, 1}, VK_IMAGE_TYPE_2D, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL);
        VkResult result = image.create();
        CHECK(result == VK_SUCCESS);
        
        cvk::basic::ImageView image_view(device);
        image_view.setup(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
        CHECK(result == VK_SUCCESS);

        VkMemoryRequirements mem_req;
        __cvk::get_memory_requirement(device, image, mem_req);
        VkDeviceMemory memory;
        VkMemoryAllocateInfo memory_alloc_info = {};
        __cvk::get_memory_allocate_info(device, device.get_memory_properties(), mem_req, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memory_alloc_info);
        result = __cvk::alloc_memory(device, memory_alloc_info, memory);
        CHECK(result == VK_SUCCESS);
        result = vkBindImageMemory(device, image, memory, 0);
        CHECK(result == VK_SUCCESS);

        result = image_view.create(image);
        
        __cvk::free_memory(device, memory);
    };

    test_color_image();

TEST_FUNC_END
