#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/memory.h"
#include "cvk/swapchain.h"
#include "cvk/shader.h"
#include "cvk/buffer.h"
#include "cvk/framebuffer.h"
#include "cvk/descriptor.h"
#include "cvk/render_pass.h"
#include "cvk/pipeline_graphics.h"
#include "cvk/image.h"
#include "cvk/buffer.h"
#include "cvk/semaphore.h"
#include "cvk/fence.h"
#include "cvk/command_pool.h"
#include "cvk/command_buffer.h"

#include "cvk/image/sampler.h"
#include "cvk/command/queue.h"
#include "cvk/image/image_view.h"
#include "cvk/pipe/pipeline_layout.h"

#include "cvk/initialize/command_initialize.h"
#include "cvk/initialize/memory_initialize.h"
#include "cvk/initialize/swapchain_initialize.h"
#include "cvk/initialize/surface_initialize.h"
#include "cvk/initialize/pipe_initialize.h"

#include "utils/file.h"
#include "utils/timer.h"

#include "render/render_target.h"

#include <memory>

#include "load_texture.h"
#include "load_model.h"


#ifdef WIN32
    #include "win32/surface_win32.h"
    #include "win32/win.h"
#elif linux
    #include <xcb/xcb.h>
    #include "linux/win_xcb.h"
    #include "linux/surface_xcb.h"
#endif

namespace
{
#ifdef WIN32
inline bool is_key_down_wasd(char key)
{
    return (GetAsyncKeyState(static_cast<int>(key)) & 0x8000) != 0;
}
#elif linux
inline bool xcb_key_down(xcb_connection_t* conn, uint8_t keycode)
{
    xcb_query_keymap_cookie_t cookie = xcb_query_keymap(conn);
    xcb_query_keymap_reply_t* reply = xcb_query_keymap_reply(conn, cookie, nullptr);
    if (!reply) {
        return false;
    }
    bool down = (reply->keys[keycode / 8] & (1u << (keycode % 8))) != 0;
    free(reply);
    return down;
}

inline bool is_key_down_wasd(XCBWindow const& win, char key)
{
    // Common X11 keycodes for US keyboard layout.
    constexpr uint8_t KEY_W = 25;
    constexpr uint8_t KEY_A = 38;
    constexpr uint8_t KEY_S = 39;
    constexpr uint8_t KEY_D = 40;

    switch (key) {
    case 'W': return xcb_key_down(win.get_connection(), KEY_W);
    case 'A': return xcb_key_down(win.get_connection(), KEY_A);
    case 'S': return xcb_key_down(win.get_connection(), KEY_S);
    case 'D': return xcb_key_down(win.get_connection(), KEY_D);
    default: return false;
    }
}
#endif
}

int main()
{
    // Create Vulkan instance with surface + debug extensions.
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

    cvk::Instance instance(instance_extensions, instance_layers);
    std::vector<VkPhysicalDevice>&& devices = instance.get_all_physical_device();
    cvk::Device device(devices[0]);
    device.add_extensions(
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    );
    device.create(VK_QUEUE_GRAPHICS_BIT);

    uint32_t width = 1024;
    uint32_t height = 720;

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

    // Attachments are indexed later by render pass setup.
    std::vector<VkClearValue> clear_values(8);
    clear_values[7].depthStencil.depth = 1.0f;
    clear_values[7].depthStencil.stencil = 0;

    VkRect2D render_area = {{0, 0}, {width, height}};
    VkViewport viewport = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };

    std::vector<VkSurfaceFormatKHR> formats;
    __cvk::get_surface_formats(device.get_physical_device(), surface, formats);
    CVK_ASSERT(formats.size() > 0);
    auto color_format = formats[0].format;
    cvk::Swapchain swapchain(device, device.get_physical_device(), surface, { VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR }, formats[0]);
    swapchain.create();
    VkExtent2D swapchain_extent = swapchain.get_extent();
    width = swapchain_extent.width;
    height = swapchain_extent.height;
    render_area = {{0, 0}, {width, height}};
    viewport = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };

    uint32_t graphics_index = UINT32_MAX, present_index = UINT32_MAX;
    __cvk::get_queue_family_index(device.get_physical_device(), VK_QUEUE_GRAPHICS_BIT, graphics_index);
    __cvk::get_queue_family_index_present_support(device.get_physical_device(), surface, VK_QUEUE_GRAPHICS_BIT, present_index);
    CVK_ASSERT(graphics_index != UINT32_MAX);
    CVK_ASSERT(present_index != UINT32_MAX);

    // Opaque GBuffer: position/normal/albedo.
    auto position_attachment = std::make_unique<cvk::ColorAttachInput2D>(device);
    CVK_ASSERT(position_attachment->create(device.get_memory_properties(), VK_FORMAT_R16G16B16A16_SFLOAT, width, height) == VK_SUCCESS);
    auto normal_attachment = std::make_unique<cvk::ColorAttachInput2D>(device);
    CVK_ASSERT(normal_attachment->create(device.get_memory_properties(), VK_FORMAT_R16G16B16A16_SFLOAT, width, height) == VK_SUCCESS);
    auto albedo_attachment = std::make_unique<cvk::ColorAttachInput2D>(device);
    CVK_ASSERT(albedo_attachment->create(device.get_memory_properties(), VK_FORMAT_R8G8B8A8_UNORM, width, height) == VK_SUCCESS);

    // Transparent GBuffer: store a second geometry pass separately.
    auto transparent_position_attachment = std::make_unique<cvk::ColorAttachInput2D>(device);
    CVK_ASSERT(transparent_position_attachment->create(device.get_memory_properties(), VK_FORMAT_R16G16B16A16_SFLOAT, width, height) == VK_SUCCESS);
    auto transparent_normal_attachment = std::make_unique<cvk::ColorAttachInput2D>(device);
    CVK_ASSERT(transparent_normal_attachment->create(device.get_memory_properties(), VK_FORMAT_R16G16B16A16_SFLOAT, width, height) == VK_SUCCESS);
    auto transparent_albedo_attachment = std::make_unique<cvk::ColorAttachInput2D>(device);
    CVK_ASSERT(transparent_albedo_attachment->create(device.get_memory_properties(), VK_FORMAT_R8G8B8A8_UNORM, width, height) == VK_SUCCESS);

    auto depth = std::make_unique<cvk::DepthAttachment2D>(device);
    CVK_ASSERT(depth->create(device.get_memory_properties(), VK_FORMAT_D16_UNORM, width, height) == VK_SUCCESS);

    cvk::Sampler sampler(device);
    sampler.create();

    // Attachment ids: 0..2 opaque GBuffer, 3..5 transparent GBuffer,
    // 6 swapchain color, 7 depth.
    cvk::RenderTarget target;
    // Subpass 0: render opaque geometry into opaque GBuffer.
    auto& opaque = target.add_element();
    auto position = opaque.output().add_color(0, *position_attachment);
    auto normal = opaque.output().add_color(1, *normal_attachment);
    auto albedo = opaque.output().add_color(2, *albedo_attachment);
    opaque.output().set_depth(7, *depth);
    // Subpass 1: render transparent geometry into transparent GBuffer.
    auto& transparent = target.add_element();
    auto transparent_position = transparent.output().add_color(3, *transparent_position_attachment);
    auto transparent_normal = transparent.output().add_color(4, *transparent_normal_attachment);
    auto transparent_albedo = transparent.output().add_color(5, *transparent_albedo_attachment);
    transparent.output().set_depth(7, *depth);
    // Subpass 2: lighting/composition, read both GBuffer sets as input attachments.
    auto& composition = target.add_element();
    composition.input()
        .add(position).add(normal).add(albedo)
        .add(transparent_position).add(transparent_normal).add(transparent_albedo);
    composition.output().add_present(6, color_format);
    composition.output().set_depth(7, *depth);
    // Keep write ordering between the first two color-output passes.
    target.add_dependency(0, 1,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    // Make subpass 1 color writes visible to subpass 2 fragment reads.
    target.add_dependency(1, 2,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_SHADER_READ_BIT);

    cvk::RenderPass render_pass(device);
    target.setup(render_pass);
    CVK_ASSERT(render_pass.create() == VK_SUCCESS);

    std::vector<cvk::Framebuffer> framebuffers;
    auto CONST_REFERENCE images = swapchain.get_images();
    std::vector<cvk::ColorImageView2D> image_views2d;
    for (auto i : images) {
        // One framebuffer per swapchain image, sharing offscreen GBuffer attachments.
        CVK_ASSERT(image_views2d.emplace_back(device).create(swapchain.get_format(), i) == VK_SUCCESS);
        framebuffers.emplace_back(device, render_pass, width, height).attaches(
            (VkImageView)*position_attachment,
            (VkImageView)*normal_attachment,
            (VkImageView)*albedo_attachment,
            (VkImageView)*transparent_position_attachment,
            (VkImageView)*transparent_normal_attachment,
            (VkImageView)*transparent_albedo_attachment,
            (VkImageView)image_views2d.back(),
            (VkImageView)*depth);
        CVK_ASSERT(framebuffers.back().create() == VK_SUCCESS);
    }

    glm::vec3 view_pos = glm::vec3(0, 20, -20);
    // MVP-style matrices packed into one UBO used by geometry passes.
    std::vector<glm::mat4> ubo = {
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0)),
        glm::lookAt(view_pos, glm::vec3(0, 0, 0), glm::vec3(0, -1, 0)),
        glm::perspective(glm::radians(60.f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f)
    };
    uint32_t ubo_size = sizeof(glm::mat4) * ubo.size();

    cvk::UniformBufferWritable uniform_buffer(device);
    CVK_ASSERT(uniform_buffer.create(device.get_memory_properties(), ubo_size) == VK_SUCCESS);
    CVK_ASSERT(uniform_buffer.upload(ubo.data(), ubo_size) == VK_SUCCESS);

    cvk::Texture2D texture(device);
    load_texture(device, "texture/colored_glass_rgba.ktx", graphics_index, texture);

    // Load opaque and transparent mesh groups separately.
    std::vector<NodeBuffers> building_buffers;
    load_vertex(device, "model/samplebuilding.gltf", building_buffers);
    std::vector<NodeBuffers> building_glass_buffers;
    load_vertex(device, "model/samplebuilding_glass.gltf", building_glass_buffers);
    // load_vertex(device, "model/retroufo_glow.gltf", building_buffers);
    
    cvk::Descriptor descriptor(device);
    // One descriptor set per node: global matrix UBO + node transform + texture.
    for (size_t i = 0; i < building_buffers.size(); ++i) {
        descriptor.add_layout()
            .set(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1)
            .set(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1)
            .set(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
    }
    CVK_ASSERT(descriptor.create() == VK_SUCCESS);

    for (uint32_t i = 0; i < building_buffers.size(); ++i) {
        descriptor[i].write(0, uniform_buffer.get_descriptor_info());
        descriptor[i].write(1, building_buffers[i].transform.get_descriptor_info());
        descriptor[i].write(2, texture.get_descriptor_info(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, sampler));
    }
    cvk::PipelineLayout layout(device);
    layout.attaches(static_cast<VkDescriptorSetLayout>(descriptor[0].get_layout()));
    CVK_ASSERT(layout.create() == VK_SUCCESS);

    cvk::Shader vert_shader(device, utils::load_file("shader/texture.vert.spv"));
    CVK_ASSERT(vert_shader.create() == VK_SUCCESS);
    cvk::Shader gbuff_frag_shader(device, utils::load_file("shader/gbuff.frag.spv"));
    CVK_ASSERT(gbuff_frag_shader.create() == VK_SUCCESS);
    cvk::Shader gbufft_frag_shader(device, utils::load_file("shader/gbuff_transparent.frag.spv"));
    CVK_ASSERT(gbufft_frag_shader.create() == VK_SUCCESS);

    // Pipeline for subpass 0 (opaque GBuffer write).
    cvk::GraphicsPipeline obj_pipeline(device);
    obj_pipeline.vertex_input().add_binding(0, sizeof(Vertex))
        .add_attribute(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, POSITION))
        .add_attribute(1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, NORMAL))
        .add_attribute(2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, TEXCOORD_0))
        .add_attribute(3, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, COLOR_0));
    obj_pipeline.viewport().attaches(render_area, viewport);
    obj_pipeline.color_blend()
        .attach(0xf, false)
        .attach(0xf, false)
        .attach(0xf, false);
    obj_pipeline.shader()
        .attach(VK_SHADER_STAGE_VERTEX_BIT, vert_shader)
        .attach(VK_SHADER_STAGE_FRAGMENT_BIT, gbuff_frag_shader);
    CVK_ASSERT(obj_pipeline.create(render_pass, layout) == VK_SUCCESS);

    // Descriptor and pipeline layout for transparent nodes.
    cvk::Descriptor glass_descriptor(device);
    for (size_t i = 0; i < building_glass_buffers.size(); ++i) {
        glass_descriptor.add_layout()
            .set(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1)
            .set(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1)
            .set(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
    }
    CVK_ASSERT(glass_descriptor.create() == VK_SUCCESS);
    for (uint32_t i = 0; i < building_glass_buffers.size(); ++i) {
        glass_descriptor[i].write(0, uniform_buffer.get_descriptor_info());
        glass_descriptor[i].write(1, building_glass_buffers[i].transform.get_descriptor_info());
        glass_descriptor[i].write(2, texture.get_descriptor_info(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, sampler));
    }

    // Reuse most states from opaque pipeline, switch fragment shader + subpass index.
    cvk::GraphicsPipeline transparent_pipeline = obj_pipeline;
    cvk::PipelineLayout transparent_layout(device);
    transparent_layout.attaches(static_cast<VkDescriptorSetLayout>(glass_descriptor[0].get_layout()));
    CVK_ASSERT(transparent_layout.create() == VK_SUCCESS);
    {
        cvk::GraphicsPipeline _temp(device);
        obj_pipeline.count_swap(_temp);
    }
    // transparent_pipeline.rasterization().polygonMode = VK_POLYGON_MODE_LINE;
    transparent_pipeline.shader().clear_all();
    transparent_pipeline.shader()
        .attach(VK_SHADER_STAGE_VERTEX_BIT, vert_shader)
        .attach(VK_SHADER_STAGE_FRAGMENT_BIT, gbufft_frag_shader);
    CVK_ASSERT(transparent_pipeline.create(render_pass, transparent_layout, 1) == VK_SUCCESS);

    struct Light {
    };
    struct CompositionUBO {
        glm::vec4 viewPos;
        glm::vec4 position;
        glm::vec3 color;
        float radius;
    } light_ubo;
    light_ubo.position = glm::vec4(20.0, 20.0, 0.0, 1.0);
    light_ubo.viewPos = glm::vec4(view_pos, 1.0);
    light_ubo.color = glm::vec3(1.0);
    light_ubo.radius = 100.0;
    cvk::UniformBufferWritable light_ubo_buffer(device);
    CVK_ASSERT(light_ubo_buffer.create(device.get_memory_properties(), sizeof(CompositionUBO)) == VK_SUCCESS);
    CVK_ASSERT(light_ubo_buffer.upload(&light_ubo, sizeof(light_ubo)) == VK_SUCCESS);

    // Composition pass reads all six GBuffer attachments + one light UBO.
    cvk::Descriptor light_descriptor(device);
    light_descriptor.add_layout()
        .set(0, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
        .set(1, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
        .set(2, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, VK_SHADER_STAGE_FRAGMENT_BIT, 1)

        .set(3, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
        .set(4, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
        .set(5, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, VK_SHADER_STAGE_FRAGMENT_BIT, 1)

        .set(6, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
    CVK_ASSERT(light_descriptor.create() == VK_SUCCESS);
    
    light_descriptor[0].write(0, position_attachment->get_descriptor_info(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
    light_descriptor[0].write(1, normal_attachment->get_descriptor_info(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
    light_descriptor[0].write(2, albedo_attachment->get_descriptor_info(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
    
    light_descriptor[0].write(3, transparent_position_attachment->get_descriptor_info(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
    light_descriptor[0].write(4, transparent_normal_attachment->get_descriptor_info(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
    light_descriptor[0].write(5, transparent_albedo_attachment->get_descriptor_info(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));

    light_descriptor[0].write(6, light_ubo_buffer.get_descriptor_info());

    cvk::PipelineLayout light_layout(device);
    light_layout.attaches(static_cast<VkDescriptorSetLayout>(light_descriptor[0].get_layout()));
    CVK_ASSERT(light_layout.create() == VK_SUCCESS);

    cvk::Shader composition_vert_shader(device, utils::load_file("shader/composition.vert.spv"));
    CVK_ASSERT(composition_vert_shader.create() == VK_SUCCESS);
    cvk::Shader composition_frag_shader(device, utils::load_file("shader/composition_transparent.frag.spv"));
    CVK_ASSERT(composition_frag_shader.create() == VK_SUCCESS);

    // Fullscreen composition pipeline (subpass 2).
    cvk::GraphicsPipeline light_pipeline(device);
    light_pipeline.viewport().attaches(render_area, viewport);
    light_pipeline.color_blend()
        .attach(0xf, false);
    // light_pipeline.set_subpass(2);
    light_pipeline.shader()
        .attach(VK_SHADER_STAGE_VERTEX_BIT, composition_vert_shader)
        .attach(VK_SHADER_STAGE_FRAGMENT_BIT, composition_frag_shader);
    CVK_ASSERT(light_pipeline.create(render_pass, light_layout, 2) == VK_SUCCESS);

    cvk::CommandPool command_pool(device, graphics_index);
    CVK_ASSERT(command_pool.create() == VK_SUCCESS);

    cvk::CommandBuffer command_buffer(device, command_pool);
    CVK_ASSERT(command_buffer.create() == VK_SUCCESS);

    cvk::Semaphore acquire_semaphore(device);
    CVK_ASSERT(acquire_semaphore.create() == VK_SUCCESS);
    cvk::Fence wait_fence(device);
    CVK_ASSERT(wait_fence.create() == VK_SUCCESS);

    cvk::Queue graphics_queue(device, graphics_index);

    auto recreate_swapchain_resources = [&]() -> bool {
        CVK_ASSERT(device.wait() == VK_SUCCESS);
        VkResult recreate_result = swapchain.recreate();
        if (recreate_result != VK_SUCCESS) {
            return false;
        }

        VkExtent2D extent = swapchain.get_extent();
        if (extent.width == 0 || extent.height == 0) {
            return false;
        }

        width = extent.width;
        height = extent.height;
        render_area = {{0, 0}, {width, height}};
        viewport = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };

        ubo[2] = glm::perspective(glm::radians(60.f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
        CVK_ASSERT(uniform_buffer.upload(ubo.data(), ubo_size) == VK_SUCCESS);

        position_attachment = std::make_unique<cvk::ColorAttachInput2D>(device);
        CVK_ASSERT(position_attachment->create(device.get_memory_properties(), VK_FORMAT_R16G16B16A16_SFLOAT, width, height) == VK_SUCCESS);
        normal_attachment = std::make_unique<cvk::ColorAttachInput2D>(device);
        CVK_ASSERT(normal_attachment->create(device.get_memory_properties(), VK_FORMAT_R16G16B16A16_SFLOAT, width, height) == VK_SUCCESS);
        albedo_attachment = std::make_unique<cvk::ColorAttachInput2D>(device);
        CVK_ASSERT(albedo_attachment->create(device.get_memory_properties(), VK_FORMAT_R8G8B8A8_UNORM, width, height) == VK_SUCCESS);

        transparent_position_attachment = std::make_unique<cvk::ColorAttachInput2D>(device);
        CVK_ASSERT(transparent_position_attachment->create(device.get_memory_properties(), VK_FORMAT_R16G16B16A16_SFLOAT, width, height) == VK_SUCCESS);
        transparent_normal_attachment = std::make_unique<cvk::ColorAttachInput2D>(device);
        CVK_ASSERT(transparent_normal_attachment->create(device.get_memory_properties(), VK_FORMAT_R16G16B16A16_SFLOAT, width, height) == VK_SUCCESS);
        transparent_albedo_attachment = std::make_unique<cvk::ColorAttachInput2D>(device);
        CVK_ASSERT(transparent_albedo_attachment->create(device.get_memory_properties(), VK_FORMAT_R8G8B8A8_UNORM, width, height) == VK_SUCCESS);

        depth = std::make_unique<cvk::DepthAttachment2D>(device);
        CVK_ASSERT(depth->create(device.get_memory_properties(), VK_FORMAT_D16_UNORM, width, height) == VK_SUCCESS);

        light_descriptor[0].write(0, position_attachment->get_descriptor_info(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
        light_descriptor[0].write(1, normal_attachment->get_descriptor_info(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
        light_descriptor[0].write(2, albedo_attachment->get_descriptor_info(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
        light_descriptor[0].write(3, transparent_position_attachment->get_descriptor_info(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
        light_descriptor[0].write(4, transparent_normal_attachment->get_descriptor_info(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
        light_descriptor[0].write(5, transparent_albedo_attachment->get_descriptor_info(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));

        image_views2d.clear();
        framebuffers.clear();
        auto CONST_REFERENCE recreated_images = swapchain.get_images();
        for (auto image : recreated_images) {
            CVK_ASSERT(image_views2d.emplace_back(device).create(swapchain.get_format(), image) == VK_SUCCESS);
            framebuffers.emplace_back(device, render_pass, width, height).attaches(
                (VkImageView)*position_attachment,
                (VkImageView)*normal_attachment,
                (VkImageView)*albedo_attachment,
                (VkImageView)*transparent_position_attachment,
                (VkImageView)*transparent_normal_attachment,
                (VkImageView)*transparent_albedo_attachment,
                (VkImageView)image_views2d.back(),
                (VkImageView)*depth);
            CVK_ASSERT(framebuffers.back().create() == VK_SUCCESS);
        }

        return true;
    };

    utils::Stopwatch benchmark;
    benchmark.start();
    uint32_t time_i = 0;
    uint32_t time = 0;

    uint32_t msg = 0;
    while (win.poll_event(msg)) {

        glm::vec3 forward = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - view_pos);
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, -1.0f, 0.0f)));
        constexpr float move_speed = 0.2f;

    #ifdef WIN32
        if (is_key_down_wasd('W')) view_pos += forward * move_speed;
        if (is_key_down_wasd('S')) view_pos -= forward * move_speed;
        if (is_key_down_wasd('A')) view_pos -= right * move_speed;
        if (is_key_down_wasd('D')) view_pos += right * move_speed;
    #elif linux
        if (is_key_down_wasd(win, 'W')) view_pos += forward * move_speed;
        if (is_key_down_wasd(win, 'S')) view_pos -= forward * move_speed;
        if (is_key_down_wasd(win, 'A')) view_pos -= right * move_speed;
        if (is_key_down_wasd(win, 'D')) view_pos += right * move_speed;
    #endif

        // Update camera-dependent UBOs for this frame.
        ubo[1] = glm::lookAt(view_pos, glm::vec3(0, 0, 0), glm::vec3(0, -1, 0));
        CVK_ASSERT(uniform_buffer.upload(ubo.data(), ubo_size) == VK_SUCCESS);
        light_ubo.viewPos = glm::vec4(view_pos, 1.0f);
        CVK_ASSERT(light_ubo_buffer.upload(&light_ubo, sizeof(light_ubo)) == VK_SUCCESS);

        // Acquire current swapchain image and record a full frame.

        uint32_t cur_index = UINT32_MAX;
        VkResult acquire_result = swapchain.acquire(cur_index, acquire_semaphore);
        if (acquire_result == VK_ERROR_OUT_OF_DATE_KHR || acquire_result == VK_SUBOPTIMAL_KHR) {
            recreate_swapchain_resources();
            win.free_event();
            continue;
        }
        CVK_ASSERT(acquire_result == VK_SUCCESS);

        CVK_ASSERT(command_buffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) == VK_SUCCESS);

        VkRenderPassBeginInfo begin_renderpass_info;
        __cvk::get_default_begin_renderpass_info(render_pass, framebuffers[cur_index], clear_values, render_area, begin_renderpass_info);
        command_buffer.cmd().begin_renderpass(begin_renderpass_info);

        // Subpass 0: opaque geometry -> opaque GBuffer.
        command_buffer.cmd().bind_pipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, obj_pipeline);
        for (uint32_t i = 0; i < building_buffers.size(); ++i) 
        {
            command_buffer.cmd().bind_descriptor_sets(VK_PIPELINE_BIND_POINT_GRAPHICS, layout, { descriptor[i] });
            command_buffer.cmd().bind_vertex_buffer(building_buffers[i].vertex);
            command_buffer.cmd().bind_index_buffer(VK_INDEX_TYPE_UINT16, building_buffers[i].index);
            command_buffer.cmd().draw_indexed(building_buffers[i].index.get_memory_size() / sizeof(uint16_t));
        }

        command_buffer.cmd().next_subpass();

    // Subpass 1: transparent geometry -> transparent GBuffer.
        command_buffer.cmd().bind_pipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, transparent_pipeline);
        for (uint32_t i = 0; i < building_glass_buffers.size(); ++i) 
        {
            command_buffer.cmd().bind_descriptor_sets(VK_PIPELINE_BIND_POINT_GRAPHICS, transparent_layout, { glass_descriptor[i] });
            command_buffer.cmd().bind_vertex_buffer(building_glass_buffers[i].vertex);
            command_buffer.cmd().bind_index_buffer(VK_INDEX_TYPE_UINT16, building_glass_buffers[i].index);
            command_buffer.cmd().draw_indexed(building_glass_buffers[i].index.get_memory_size() / sizeof(uint16_t));
        }

        command_buffer.cmd().next_subpass();
        
        {
            // Subpass 2: fullscreen composition using input attachments.
            command_buffer.cmd().bind_pipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, light_pipeline);
            command_buffer.cmd().bind_descriptor_sets(VK_PIPELINE_BIND_POINT_GRAPHICS, light_layout, { light_descriptor[0] });
            command_buffer.cmd().draw(6);
        }

        command_buffer.cmd().end_renderpass();
        command_buffer.end();

        // Submit and wait on a fence for simple frame pacing.
        graphics_queue
            .set_wait(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, { acquire_semaphore })
            .submit({ command_buffer }, wait_fence);

        CVK_ASSERT(wait_fence.wait() == VK_SUCCESS);
        CVK_ASSERT(wait_fence.reset() == VK_SUCCESS);

        VkResult present_result = swapchain.present(graphics_queue, {});
        if (present_result == VK_ERROR_OUT_OF_DATE_KHR || present_result == VK_SUBOPTIMAL_KHR) {
            recreate_swapchain_resources();
            win.free_event();
            continue;
        }
        CVK_ASSERT(present_result == VK_SUCCESS);

        time += benchmark.lap();
        if (time_i++ == 100) {
            std::cout << "fps : " << 1000.0f / (static_cast<float>(time) / 100.0f) << "\n";
            time = 0;
            time_i = 0;
        }

        win.free_event();
    }
}
