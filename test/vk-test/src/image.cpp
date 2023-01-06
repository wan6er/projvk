#include "test_header.h"

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/memory.h"
#include "cvk/buffer.h"
#include "cvk/image/image_view.h"
#include "cvk/initialize/surface_initialize.h"
#include "cvk/initialize/image_initialize.h"
#include "cvk/initialize/memory_initialize.h"

#ifdef WIN32
#include "win32/surface_win32.h"
#include "win32/win.h"
#endif

TEST_FUNC_BEGIN("image")

    std::vector<std::string> instance_extensions = {
#ifdef WIN32
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
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
    cvk::Device device(devices[0], device_extensions, {}, VK_QUEUE_GRAPHICS_BIT);

    uint32_t width = 1024, height = 720;
#ifdef WIN32
    Windows win("image", width, height);
    cvk::SurfaceWin32 surface(instance, win.instance(), win);
#else
#error unsupport platform
#endif

    auto test_color_image = [&] {
        cvk::ImageView2D image_view(device);
        VkResult result = image_view.create_image(VK_FORMAT_R8G8B8A8_UNORM, 1000, 700 , VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL);
        CHECK(result == VK_SUCCESS);

        VkMemoryRequirements mem_req;
        __cvk::get_memory_requirement(device, image_view, mem_req);
        VkDeviceMemory memory;
        result = __cvk::alloc_memory(device, device.get_memory_properties(), mem_req, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memory);
        CHECK(result == VK_SUCCESS);
        result = vkBindImageMemory(device, image_view, memory, 0);
        CHECK(result == VK_SUCCESS);

        result = image_view.create_image_view(VK_IMAGE_ASPECT_COLOR_BIT);
        CHECK(result == VK_SUCCESS);

        __cvk::free_memory(device, memory);

        cvk::ImageView2D image_view2(image_view);
        CHECK((static_cast<VkImageView>(image_view2) != VK_NULL_HANDLE));
    };

    auto test_depth_image = [&]{
        cvk::ImageView2D depth_image_view(device);
        VkResult result = depth_image_view.create_image(VK_FORMAT_D16_UNORM, 1000, 700 , VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL);
        CHECK(result == VK_SUCCESS);
        
        VkMemoryRequirements mem_req;
        __cvk::get_memory_requirement(device, depth_image_view, mem_req);
        VkDeviceMemory depth_memory;
        result = __cvk::alloc_memory(device, device.get_memory_properties(), mem_req, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depth_memory);
        CHECK(result == VK_SUCCESS);
        result = vkBindImageMemory(device, depth_image_view, depth_memory, 0);
        CHECK(result == VK_SUCCESS);

        depth_image_view.create_image_view(VK_IMAGE_ASPECT_DEPTH_BIT);
        __cvk::free_memory(device, depth_memory);
    };

    auto test_texture = [&] {
        cvk::ImageView2D image_view(device);
        VkResult result = image_view.create_image(VK_FORMAT_R8G8B8A8_UNORM, 1000, 700 , VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_TILING_LINEAR);
        CHECK(result == VK_SUCCESS);

        VkMemoryRequirements mem_req;
        __cvk::get_memory_requirement(device, image_view, mem_req);

        cvk::Memory memory(device, device.get_memory_properties(), mem_req, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        result = memory.allocate();
        CHECK(result == VK_SUCCESS);
        result = memory.bind(image_view);
        CHECK(result == VK_SUCCESS);

        result = image_view.create_image_view(VK_IMAGE_ASPECT_COLOR_BIT);
        CHECK(result == VK_SUCCESS);

    };

    auto test_texture_with_staging_buffer = [&] {
        cvk::ImageView2D image_view(device);
        VkResult result = image_view.create_image(VK_FORMAT_R8G8B8A8_UNORM, 1000, 700 , VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_TILING_OPTIMAL);
        CHECK(result == VK_SUCCESS);

        cvk::Memory image_memory(device, device.get_memory_properties(), image_view.get_memory_requirement(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        result = image_memory.allocate();
        CHECK(result == VK_SUCCESS);
        result = image_memory.bind(image_view);
        CHECK(result == VK_SUCCESS);

        cvk::Buffer buffer(device, image_view.get_memory_requirement().size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        buffer.create();
        cvk::Memory buffer_memory(device, device.get_memory_properties(), buffer.get_memory_requirement(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        result = buffer_memory.allocate();
        CHECK(result == VK_SUCCESS);
        result = buffer_memory.bind(buffer);
        CHECK(result == VK_SUCCESS);

        uint8_t* data = nullptr;
        result = buffer_memory.map(data);
        CHECK(result == VK_SUCCESS);

        buffer_memory.unmap();

    };
    test_color_image();
    test_depth_image();
    test_texture();
    test_texture_with_staging_buffer();

TEST_FUNC_END
