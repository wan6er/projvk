#include "test_header.h"

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/surface.h"
#include "cvk/memory.h"
#include "cvk/swapchain.h"
#include "cvk/image.h"
#include "cvk/image/image_view.h"
#include "cvk/image/sampler.h"
#include "cvk/descriptor.h"
#include "cvk/descriptor/descriptor_pool.h"
#include "cvk/descriptor/descriptor_set.h"
#include "cvk/descriptor/descriptor_set_layout.h"
#include "cvk/descriptor/write_descriptor_set.h"
#include "cvk/descriptor/descriptor_set_writer.h"
#include "cvk/initialize/command_initialize.h"
#include "cvk/initialize/surface_initialize.h"
#include "cvk/initialize/descriptor_initialize.h"
// #include "cvk/initialize/pipe_initialize.h"

#ifdef WIN32
#include "win32/surface_win32.h"
#include "win32/win.h"
#endif

TEST_FUNC_BEGIN("descriptor")
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
    Windows win;
    win.create("descriptor", width, height);
    win.show();
    cvk::SurfaceWin32 surface(instance, win.instance(), win);
#elif linux
    XCBWindow win;
    win.create("triangle", width, height);
    win.show();
    cvk::SurfaceXCB surface(instance, win.get_connection(), win.get_window());
#else
#error unsupport platform
#endif

    std::vector<VkSurfaceFormatKHR> formats;
    __cvk::get_surface_formats(device.get_physical_device(), surface, formats);
    CHECK(formats.size() > 0);
    cvk::Swapchain swapchain(device, device.get_physical_device(), surface, { VK_PRESENT_MODE_FIFO_KHR }, formats[0]);
    swapchain.create();

    uint32_t tex_width = 100, tex_height = 100;
    // cvk::Image2D texture(device, VK_FORMAT_R8G8B8A8_UNORM, tex_width, tex_height, VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_TILING_LINEAR);

    cvk::StandardTexture2D texture(device);
    texture.create(device.get_memory_properties(), VK_FORMAT_R8G8B8A8_UNORM, tex_width, tex_height);

    cvk::Sampler sampler(device);
    sampler.create();

    VkDescriptorImageInfo tex_copy_info;
    __cvk::get_default_descriptor_image_info(sampler, texture, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, tex_copy_info);

    cvk::DescriptorPool descriptor_pool(device);
    // descriptor_pool.attaches(tex_binding);
    // descriptor_pool.set(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1);
    descriptor_pool.set(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 2);
    CHECK(descriptor_pool.create(swapchain.get_images().size()) == VK_SUCCESS);

    cvk::DescriptorSet tex_set[2] = {{ device, descriptor_pool }, { device, descriptor_pool } };

    cvk::WriteDescriptorSet copy_set;
    copy_set.attaches(tex_copy_info);
    cvk::WriteDescriptorSet copy_set1;
    copy_set1.attaches(tex_copy_info);

    VkDescriptorSetLayoutBinding tex0_binding = cvk::DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VK_SHADER_STAGE_FRAGMENT_BIT, 0);
    VkDescriptorSetLayoutBinding tex1_binding = cvk::DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VK_SHADER_STAGE_FRAGMENT_BIT, 1);

    
    tex_set[0].attaches(tex0_binding, tex1_binding);
    tex_set[1].attaches(tex0_binding, tex1_binding);

    CHECK(tex_set[0].create() == VK_SUCCESS);
    CHECK(tex_set[1].create() == VK_SUCCESS);
    copy_set.setup(tex_set[0], VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 0).update(device);
    copy_set1.setup(tex_set[1], VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1).update(device);

    {
        cvk::Descriptor descriptor(device);
        descriptor.add_layout()
            .set(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1);
        descriptor.add_layout()
            .set(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1);
        CHECK(descriptor.create() == VK_SUCCESS);
    }


    {
        cvk::DescriptorSetLayout layout1(device);
        layout1.set(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1);
        layout1.set(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1);
        CHECK(layout1.create() == VK_SUCCESS);
        cvk::DescriptorSetLayout layout2(device);
        layout2.set(2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1);
        layout2.set(3, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1);

        cvk::Descriptor descriptor(device);
        descriptor.add_layout(layout1);
        descriptor.add_layout(layout1);
        descriptor.add_layout(layout2.copy());
        CHECK(descriptor.create() == VK_SUCCESS);
    }

    {
        cvk::DescriptorSetWriter writer(2);
        writer[0].setup(tex_set[0], VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1)
            .attaches(tex_copy_info);
        writer[1].setup(tex_set[1], VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1)
            .attaches(tex_copy_info);
        writer.update(device);
    }

TEST_FUNC_END
