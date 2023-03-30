#include "test_header.h"

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/memory.h"
#include "cvk/buffer.h"
#include "cvk/shader.h"
#include "cvk/pipe/subpass.h"
#include "cvk/pipe/base_render_pass.h"
#include "cvk/descriptor/descriptor_set_layout.h"
#include "cvk/descriptor/descriptor_pool.h"
#include "cvk/descriptor/descriptor_set.h"
#include "cvk/pipe/pipeline_layout.h"
#include "cvk/render_pass.h"
#include "cvk/graphics_pipeline.h"
#include "cvk/image/image_view.h"
#include "cvk/initialize/pipe_initialize.h"
#include "cvk/initialize/surface_initialize.h"
#include "cvk/initialize/image_initialize.h"
#include "cvk/initialize/memory_initialize.h"
#include "utils/file.h"

#ifdef WIN32
#include "win32/surface_win32.h"
#include "win32/win.h"
#endif

void test_pipeline_attachment() 
{
    // cvk::VertexInputState vi;
    cvk::GraphicsPipeline test_pipeline(nullptr, nullptr, nullptr);
    test_pipeline.vertex_input()
        .add_binding(0, 32)
            .add_attribute(0, VK_FORMAT_R32G32B32_SFLOAT, 0)
            .add_attribute(1, VK_FORMAT_R32G32B32_SFLOAT, 12);
    test_pipeline.vertex_input()
        .add_binding(1, 32)
            .add_attribute(0, VK_FORMAT_R32G32B32_SFLOAT, 0)
            .add_attribute(1, VK_FORMAT_R32G32B32_SFLOAT, 12);
    // test_pipeline.attach(vi);
    // test_pipeline.clear_all();
    test_pipeline.attaches(VkPipelineShaderStageCreateInfo(), VkPipelineShaderStageCreateInfo());
    CHECK((test_pipeline.get_attachments().size() == 2));
    test_pipeline.viewport().attaches(VkViewport(), VkRect2D(), VkViewport());
    CHECK((test_pipeline.viewport().viewportCount == 2));
    CHECK((test_pipeline.viewport().scissorCount == 1));
    test_pipeline.dynamic().attaches(VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR);
    CHECK((test_pipeline.dynamic().dynamicStateCount == 2));
    VkPipelineColorBlendAttachmentState att_state = {};
    att_state.colorWriteMask = 0xf;
    test_pipeline.color_blend().attaches(att_state);
    CHECK((test_pipeline.color_blend().attachmentCount == 1));
}

void test_base_renderpass(VkDevice device)
{
    
    cvk::Subpass subpass(VK_PIPELINE_BIND_POINT_GRAPHICS);
    subpass.add_color(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .set_depth(1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    VkAttachmentDescription color_attachment_desc;
    __cvk::get_default_attachment_description(VK_FORMAT_B8G8R8A8_UNORM, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, color_attachment_desc);
    VkAttachmentDescription depth_attachment_desc;
    __cvk::get_default_attachment_description(VK_FORMAT_D16_UNORM, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, depth_attachment_desc);
    cvk::BaseRenderPass render_pass(device);
    render_pass.attaches(color_attachment_desc, depth_attachment_desc, subpass.get_description());
    render_pass.clear_all();
    render_pass.attaches(color_attachment_desc, depth_attachment_desc, subpass.get_description());
    VkResult result = render_pass.create();
    CHECK(result == VK_SUCCESS);
}


void test_renderpass(VkDevice device)
{
    cvk::RenderPass renderpass(device);
    renderpass.add_subpass(VK_PIPELINE_BIND_POINT_GRAPHICS)
        .add_color(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .set_depth(1);
    renderpass.add_subpass_dependency(VK_SUBPASS_EXTERNAL, 0)
        .set_src(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0)
        .set_dst(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    renderpass.add_subpass_dependency(0, VK_SUBPASS_EXTERNAL)
        .set_src(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)
        .set_dst(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0);
    renderpass
        .add_attachment(VK_FORMAT_B8G8R8A8_UNORM, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
        .add_attachment(VK_FORMAT_D16_UNORM, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    CHECK(renderpass.create() == VK_SUCCESS);
}

void test_descriptor(VkDevice device)
{
    VkDescriptorSetLayoutBinding binding0 = cvk::DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 0, 1);
    VkDescriptorSetLayoutBinding binding1 = cvk::DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 1, 1);

    cvk::DescriptorPool dp(device);
    dp.set(binding0);
    VkResult result = dp.create(2);
    CHECK(result == VK_SUCCESS);

    cvk::DescriptorSet set(device, dp);
    set.attaches(binding0, binding1);
    result = set.create();
    CHECK(result == VK_SUCCESS);
    // set.attaches()
}

TEST_FUNC_BEGIN("renderpass")

    
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

    uint32_t width = 1024, height = 720;

#ifdef WIN32
    Windows win;
    win.create("triangle", width, height);
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

    test_pipeline_attachment();
    test_descriptor(device);
    test_base_renderpass(device);
    test_renderpass(device);

TEST_FUNC_END

TEST_FUNC_BEGIN("graphics pipeline")

    
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

    uint32_t width = 1024, height = 720;
    std::vector<VkSurfaceFormatKHR> formats;
#ifdef WIN32
    Windows win;
    win.create("pipeline", width, height);
    win.show();
    cvk::SurfaceWin32 surface(instance, win.instance(), win);
#elif linux
    XCBWindow win;
    win.create("pipeline", width, height);
    win.show();
    cvk::SurfaceXCB surface(instance, win.get_connection(), win.get_window());
#else
#error unsupport platform
#endif
    __cvk::get_surface_formats(device.get_physical_device(), surface, formats);
    
    cvk::Subpass subpass(VK_PIPELINE_BIND_POINT_GRAPHICS);
    subpass.add_color(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .set_depth(1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    VkAttachmentDescription color_attachment_desc;
    __cvk::get_default_attachment_description(formats[0].format, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, color_attachment_desc);
    VkAttachmentDescription depth_attachment_desc;
    __cvk::get_default_attachment_description(VK_FORMAT_D16_UNORM, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, depth_attachment_desc);
    cvk::BaseRenderPass render_pass(device);
    render_pass.attaches(color_attachment_desc, depth_attachment_desc, subpass.get_description());
    render_pass.clear_all();
    render_pass.attaches(color_attachment_desc, depth_attachment_desc, subpass.get_description());
    VkResult result = render_pass.create();
    CHECK(result == VK_SUCCESS);

    struct Vertex {
        float position[3];
        float color[3];
    };

    cvk::Shader vert_shader(device, utils::load_file("shader/triangle.vert.spv"));
    CHECK(vert_shader.create() == VK_SUCCESS);
    cvk::Shader frag_shader(device, utils::load_file("shader/triangle.frag.spv"));
    CHECK(frag_shader.create() == VK_SUCCESS);

    VkDescriptorSetLayoutBinding binding0 = cvk::DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0, 1);
    VkDescriptorSetLayoutBinding binding1 = cvk::DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 1, 1);

    cvk::DescriptorPool descriptor_pool(device);
    descriptor_pool.set(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2);
    result = descriptor_pool.create(2);
    CHECK(result == VK_SUCCESS);

    cvk::DescriptorSet set(device, descriptor_pool);
    set.attaches(binding0, binding1);
    result = set.create();
    cvk::PipelineLayout layout(device);
    layout.attaches(set.get_layout());
    CHECK(layout.create() == VK_SUCCESS);


    cvk::GraphicsPipeline pipeline(device, render_pass, layout);
    pipeline.vertex_input().add_binding(0, sizeof(Vertex))
        .add_attribute(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position))
        .add_attribute(1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color));
    pipeline.viewport().set_size(1, 1);
    pipeline.dynamic().attaches(VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR);
    pipeline.color_blend().attach(0xf, false);
    pipeline.shader()
        .attach(VK_SHADER_STAGE_VERTEX_BIT, vert_shader)
        .attach(VK_SHADER_STAGE_FRAGMENT_BIT, frag_shader);
    CHECK(pipeline.create() == VK_SUCCESS);
    
TEST_FUNC_END
