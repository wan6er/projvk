#include "test_header.h"

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/memory.h"
#include "cvk/buffer.h"
#include "cvk/shader.h"
#include "cvk/pipe/subpass.h"
#include "cvk/pipe/render_pass.h"
#include "cvk/pipe/descriptor_set_layout.h"
#include "cvk/pipe/descriptor_pool.h"
#include "cvk/pipe/descriptor_set.h"
#include "cvk/pipe/pipeline_layout.h"
#include "cvk/pipe/graphics_pipeline.h"
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
    cvk::VertexInputState vi;
    cvk::GraphicsPipeline test_pipeline(nullptr, nullptr);
    test_pipeline.attach(vi);
    // test_pipeline.clear_all();
    test_pipeline.attaches(VkPipelineShaderStageCreateInfo(), VkPipelineShaderStageCreateInfo());
    CHECK((test_pipeline.get_attachments().size() == 2));
    test_pipeline.viewport().attaches(VkViewport(), VkRect2D(), VkViewport());
    CHECK((test_pipeline.viewport().viewportCount == 2));
    CHECK((test_pipeline.viewport().scissorCount == 1));
    test_pipeline.dynamic().attaches(VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR);
    CHECK((test_pipeline.dynamic().dynamicStateCount == 2));
    VkPipelineColorBlendAttachmentState att_state = { .colorWriteMask = 0xf, };
    test_pipeline.color_blend().attaches(att_state);
    CHECK((test_pipeline.color_blend().attachmentCount == 1));
}

void test_renderpass(VkDevice device)
{
    const VkAttachmentReference color_reference0 = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };
    const VkAttachmentReference depth_reference = {
        .attachment = 1,
        .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };
    
    cvk::Subpass subpass(VK_PIPELINE_BIND_POINT_GRAPHICS);
    subpass.color().attaches(color_reference0);
    subpass.depth().attach(depth_reference);

    VkAttachmentDescription color_attachment_desc;
    __cvk::get_default_attachment_description(VK_FORMAT_B8G8R8A8_UNORM, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, color_attachment_desc);
    VkAttachmentDescription depth_attachment_desc;
    __cvk::get_default_attachment_description(VK_FORMAT_D16_UNORM, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, depth_attachment_desc);
    cvk::RenderPass render_pass;
    render_pass.attaches(color_attachment_desc, depth_attachment_desc, subpass.get_description());
    render_pass.clear_all();
    render_pass.attaches(color_attachment_desc, depth_attachment_desc, subpass.get_description());
    VkResult result = render_pass.create(device);
    CHECK(result == VK_SUCCESS);

}

void test_descriptor(VkDevice device)
{
    cvk::DescriptorSetLayout dsl;
    VkDescriptorSetLayoutBinding binding0 = cvk::DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 0, 1);
    VkDescriptorSetLayoutBinding binding1 = cvk::DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 1, 1);
    dsl.attaches(binding0, binding1);
    VkResult result = dsl.create(device);
    CHECK(result == VK_SUCCESS);

    cvk::DescriptorPool dp(2);
    dp.set(binding0);
    result = dp.create(device);
    CHECK(result == VK_SUCCESS);

    cvk::DescriptorSet set(dp, dsl);
    result = set.allocate(device);
    // set.attaches()
}

TEST_FUNC_BEGIN("render_pass")

    
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
    cvk::Device device(devices[0], device_extensions, {}, VK_QUEUE_GRAPHICS_BIT);

    uint32_t width = 1024, height = 720;
#ifdef WIN32
    Windows win("render_pass", width, height);
    cvk::SurfaceWin32 surface(instance, win.instance(), win);
#else
#error unsupport platform
#endif

    test_pipeline_attachment();
    test_descriptor(device);
    test_renderpass(device);

TEST_FUNC_END

TEST_FUNC_BEGIN("graphics pipeline")

    
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
    cvk::Device device(devices[0], device_extensions, {}, VK_QUEUE_GRAPHICS_BIT);

    uint32_t width = 1024, height = 720;
#ifdef WIN32
    Windows win("graphics pipeline", width, height);
    cvk::SurfaceWin32 surface(instance, win.instance(), win);
#else
#error unsupport platform
#endif

    std::vector<VkSurfaceFormatKHR> formats;
    __cvk::get_surface_formats(device.get_physical_device(), surface, formats);
    
    const VkAttachmentReference color_reference0 = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };
    const VkAttachmentReference depth_reference = {
        .attachment = 1,
        .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };
    
    cvk::Subpass subpass(VK_PIPELINE_BIND_POINT_GRAPHICS);
    subpass.color().attaches(color_reference0);
    subpass.depth().attach(depth_reference);

    VkAttachmentDescription color_attachment_desc;
    __cvk::get_default_attachment_description(formats[0].format, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, color_attachment_desc);
    VkAttachmentDescription depth_attachment_desc;
    __cvk::get_default_attachment_description(VK_FORMAT_D16_UNORM, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, depth_attachment_desc);
    cvk::RenderPass render_pass;
    render_pass.attaches(color_attachment_desc, depth_attachment_desc, subpass.get_description());
    render_pass.clear_all();
    render_pass.attaches(color_attachment_desc, depth_attachment_desc, subpass.get_description());
    VkResult result = render_pass.create(device);
    CHECK(result == VK_SUCCESS);

    struct Vertex {
        float position[3];
        float color[3];
    };

    VkVertexInputBindingDescription vertexInputBinding = {};
    vertexInputBinding.binding = 0;
    vertexInputBinding.stride = sizeof(Vertex);
    vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription positionInputAttributs;
    positionInputAttributs.binding = 0;
    positionInputAttributs.location = 0;
    positionInputAttributs.format = VK_FORMAT_R32G32B32_SFLOAT;
    positionInputAttributs.offset = offsetof(Vertex, position);
    // Attribute location 1: Color
    VkVertexInputAttributeDescription colorInputAttributs;
    colorInputAttributs.binding = 0;
    colorInputAttributs.location = 1;
    colorInputAttributs.format = VK_FORMAT_R32G32B32_SFLOAT;
    colorInputAttributs.offset = offsetof(Vertex, color);

    cvk::Shader vert_shader(utils::load_file("triangle.vert.spv"));
    CHECK(vert_shader.create(device) == VK_SUCCESS);
    cvk::Shader frag_shader(utils::load_file("triangle.frag.spv"));
    CHECK(frag_shader.create(device) == VK_SUCCESS);

    cvk::DescriptorSetLayout descriptor_layout;
    VkDescriptorSetLayoutBinding binding0 = cvk::DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0, 1);
    VkDescriptorSetLayoutBinding binding1 = cvk::DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 1, 1);
    descriptor_layout.attaches(binding0, binding1);
    CHECK(descriptor_layout.create(device) == VK_SUCCESS);

    cvk::DescriptorPool descriptor_pool(2);
    descriptor_pool.set(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2);
    result = descriptor_pool.create(device);
    CHECK(result == VK_SUCCESS);

    cvk::DescriptorSet set(descriptor_pool, descriptor_layout);
    result = set.allocate(device);
    cvk::PipelineLayout layout;
    layout.attaches(descriptor_layout.get_layout());
    CHECK(layout.create(device) == VK_SUCCESS);

    cvk::GraphicsPipeline pipeline(render_pass, layout);
    pipeline.vertex_input().attaches(vertexInputBinding, positionInputAttributs, colorInputAttributs);
    pipeline.viewport().set_size(1, 1);
    pipeline.dynamic().attaches(VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR);
    pipeline.color_blend().attach(0xf, false);
    pipeline.shader()
        .attach(VK_SHADER_STAGE_VERTEX_BIT, vert_shader)
        .attach(VK_SHADER_STAGE_FRAGMENT_BIT, frag_shader);
    CHECK(pipeline.create(device) == VK_SUCCESS);
    
TEST_FUNC_END
