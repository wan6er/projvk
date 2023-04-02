#include "test_header.h"

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/memory.h"
#include "cvk/swapchain.h"
#include "cvk/image.h"
#include "cvk/image/image_view.h"
#include "cvk/initialize/surface_initialize.h"
#include "cvk/initialize/image_initialize.h"
#include "cvk/initialize/swapchain_initialize.h"
#include "utils/vector_util.h"


#ifdef WIN32
    #include "win32/surface_win32.h"
    #include "win32/win.h"
#elif linux
    #include "linux/win_xcb.h"
    #include "linux/surface_xcb.h"
#endif


TEST_FUNC_BEGIN("swapchain")

    std::vector<std::string> instance_extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef WIN32
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif linux
        VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#endif
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };
    std::vector<std::string> instance_layers = {
        "VK_LAYER_KHRONOS_validation"
    };
    std::vector<std::string> device_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        // VK_EXT_DEBUG_MARKER_EXTENSION_NAME
    };

    cvk::Instance instance(instance_extensions, instance_layers);
    std::vector<VkPhysicalDevice>&& devices = instance.get_all_physical_device();
    VkPhysicalDeviceFeatures device_features = {};
    cvk::Device device(devices[0], device_extensions, device_features, VK_QUEUE_GRAPHICS_BIT);

    uint32_t width = 1024, height = 720;

#ifdef WIN32
    Windows win;
    win.create("swapchain", width, height);
    win.show();
    cvk::SurfaceWin32 surface(instance, win.instance(), win);
#elif linux
    XCBWindow win;
    win.create("swapchain", width, height);
    win.show();
    cvk::SurfaceXCB surface(instance, win.get_connection(), win.get_window());
#else
#error unsupport platform
#endif
    std::vector<VkSurfaceFormatKHR> formats;
    __cvk::get_surface_formats(device.get_physical_device(), surface, formats);
    CHECK(formats.size() > 0);


    {
        cvk::Swapchain swapchain(device, device.get_physical_device(), surface, { VK_PRESENT_MODE_FIFO_KHR }, formats[0]);
        swapchain.create();
        
        std::vector<VkImage> images;
        __cvk::get_swapchain_images(device, swapchain, images);
        CHECK(images.size() > 0);

        std::vector<VkImageView> image_views;
        VkImageViewCreateInfo image_view_create_info = {};
        __cvk::get_swapchain_image_view_create_info(swapchain.info(), image_view_create_info);
        for (auto image : images) {
            VkImageView image_view;
            image_view_create_info.image = image;
            VkResult result = __cvk::create_image_view(device, image_view_create_info, image_view);
            // VkResult result = __cvk::create_image_view2d(device, image, swapchain.info().imageFormat, VK_IMAGE_ASPECT_COLOR_BIT, image_view);
            CHECK(result == VK_SUCCESS);
            image_views.push_back(image_view);
        }

        for (auto view : image_views) {
            __cvk::destroy_image_view(device, view);
        }
    }
    {
        cvk::Swapchain swapchain(device, device.get_physical_device(), surface, { VK_PRESENT_MODE_FIFO_KHR }, formats[0]);
        VkResult result = swapchain.create();
        CHECK(result == VK_SUCCESS);
        
        std::vector<VkImage> images;
        __cvk::get_swapchain_images(device, swapchain, images);

        // VkImageViewCreateInfo image_view_create_info = {};
        // __cvk::get_swapchain_image_view_create_info(swapchain.info(), image_view_create_info);
        std::vector<cvk::ColorImageView2D> image_views;
        for (auto image : images) {
            result = image_views.emplace_back(device).create(swapchain.get_format(), image);
            CHECK(result == VK_SUCCESS);
        }
    }

TEST_FUNC_END