#include "test_header.h"

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/surface.h"
#include "cvk/memory.h"
#include "cvk/swapchain.h"
#include "cvk/framebuffer.h"
#include "cvk/pipe/subpass.h"
#include "cvk/pipe/base_render_pass.h"
#include "cvk/image/image_view.h"
#include "cvk/image/sampler.h"
#include "cvk/initialize/pipe_initialize.h"
#include "cvk/initialize/command_initialize.h"
#include "cvk/initialize/surface_initialize.h"
#include "utils/vector_util.h"

#ifdef WIN32
#include "win32/surface_win32.h"
#include "win32/win.h"
#endif

TEST_FUNC_BEGIN("framebuffer")
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
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        // VK_EXT_DEBUG_MARKER_EXTENSION_NAME
    };

    cvk::Instance instance(instance_extensions, instance_layers);
    std::vector<VkPhysicalDevice>&& devices = instance.get_all_physical_device();
    VkPhysicalDeviceFeatures device_features = {};
    cvk::Device device(devices[0], device_extensions, device_features, VK_QUEUE_GRAPHICS_BIT);
    
    uint32_t width = 1024;
    uint32_t height = 720;

#ifdef WIN32
    Windows win("framebuffer", width, height);
    cvk::SurfaceWin32 surface(instance, win.instance(), win);
#else
#error unsupport platform
#endif

    std::vector<VkSurfaceFormatKHR> formats;
    __cvk::get_surface_formats(device.get_physical_device(), surface, formats);

    cvk::Swapchain swapchain(device, device.get_physical_device(), surface, { VK_PRESENT_MODE_FIFO_KHR }, formats[0]);
    swapchain.create();

    cvk::Subpass subpass(VK_PIPELINE_BIND_POINT_GRAPHICS);
    subpass.add_color(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .set_depth(1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    
    VkAttachmentDescription color_attachment_desc;
    __cvk::get_default_attachment_description(formats[0].format, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, color_attachment_desc);
    VkAttachmentDescription depth_attachment_desc;
    __cvk::get_default_attachment_description(VK_FORMAT_D16_UNORM, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, depth_attachment_desc);
    cvk::BaseRenderPass render_pass(device);
    render_pass.attaches(color_attachment_desc, depth_attachment_desc, subpass.get_description());
    CHECK(render_pass.create() == VK_SUCCESS);

    cvk::ImageView2D depth_attach(device);
    CHECK(depth_attach.create(VK_FORMAT_D16_UNORM, width, height, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL) == VK_SUCCESS);
    cvk::Memory depth_mem(device);
    CHECK(depth_mem.allocate(device.get_memory_properties(), depth_attach.get_memory_requirement(), VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) == VK_SUCCESS);
    depth_mem.bind(depth_attach);
    CHECK(depth_attach.create_image_view(VK_IMAGE_ASPECT_DEPTH_BIT) == VK_SUCCESS);

    std::vector<cvk::Framebuffer> framebuffers;
    auto CONST_REFERENCE images = swapchain.get_images();
    std::vector<cvk::ImageView2D> image_views2d;
    for (auto i : images) {
        CHECK(image_views2d.emplace_back(device, i).create_image_view(swapchain.info()) == VK_SUCCESS);
        framebuffers.emplace_back(device, render_pass, width, height).attaches((VkImageView)image_views2d.back(), (VkImageView)depth_attach);
        CHECK(framebuffers.back().create() == VK_SUCCESS);
    }


TEST_FUNC_END
