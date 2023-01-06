#include "test_header.h"

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/memory.h"
#include "cvk/swapchain.h"
#include "cvk/shader.h"
#include "cvk/buffer.h"
#include "cvk/framebuffer.h"

#include "cvk/sync/sync_set.h"

#include "cvk/command/queue.h"
#include "cvk/command/command_pool.h"
#include "cvk/command/command_buffer.h"

#include "cvk/image/image_view.h"
#include "cvk/image/sampler.h"

// #include "cvk/pipe/descriptor_set_layout.h"
// #include "cvk/pipe/descriptor_set.h"
// #include "cvk/pipe/descriptor_pool.h"
#include "cvk/pipe/descriptor.h"
#include "cvk/pipe/render_pass.h"
#include "cvk/pipe/graphics_pipeline.h"
#include "cvk/pipe/pipeline_layout.h"
#include "cvk/pipe/subpass.h"

#include "cvk/initialize/command_initialize.h"
#include "cvk/initialize/memory_initialize.h"
#include "cvk/initialize/swapchain_initialize.h"
#include "cvk/initialize/surface_initialize.h"
#include "cvk/initialize/pipe_initialize.h"

#include "utils/file.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#ifdef WIN32
#include "win32/surface_win32.h"
#include "win32/win.h"
#endif

TEST_FUNC_BEGIN("triangle_test")

    std::vector<std::string> instance_extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
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

    uint32_t width = 1024;
    uint32_t height = 720;
#ifdef WIN32
    Windows win("system_test", width, height);
    cvk::SurfaceWin32 surface(instance, win.instance(), win);
#else
#error unsupport platform
#endif

    std::vector<VkSurfaceFormatKHR> formats;
    __cvk::get_surface_formats(device.get_physical_device(), surface, formats);
    CHECK(formats.size() > 0);
    cvk::Swapchain swapchain(device, device.get_physical_device(), surface, { VK_PRESENT_MODE_FIFO_KHR }, formats[0]);
    swapchain.create();

    uint32_t graphics_index = UINT32_MAX, present_index = UINT32_MAX;
    __cvk::get_queue_family_index(device.get_physical_device(), VK_QUEUE_GRAPHICS_BIT, graphics_index);
    __cvk::get_queue_family_index_present_support(device.get_physical_device(), surface, VK_QUEUE_GRAPHICS_BIT, present_index);
    CHECK(graphics_index != UINT32_MAX);
    CHECK(present_index != UINT32_MAX);

    cvk::ImageView2D depth(device);
    CHECK(depth.create_image(VK_FORMAT_D16_UNORM, width, height, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL) == VK_SUCCESS);
    cvk::Memory depth_mem(device, device.get_memory_properties(), depth.get_memory_requirement(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    CHECK(depth_mem.allocate() == VK_SUCCESS);
    depth_mem.bind(depth);
    CHECK(depth.create_image_view(VK_IMAGE_ASPECT_DEPTH_BIT) == VK_SUCCESS);

    cvk::Sampler sampler(device);
    sampler.create();

    const VkAttachmentReference color_reference0 = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };
    const VkAttachmentReference depth_reference = {
        .attachment = 1,
        .layout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL,
    };
    cvk::Subpass subpass(VK_PIPELINE_BIND_POINT_GRAPHICS);
    subpass.color().attaches(color_reference0);
    subpass.depth().attach(depth_reference);

    VkAttachmentDescription color_attachment_desc;
    __cvk::get_default_attachment_description(formats[0].format, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, color_attachment_desc);
    VkAttachmentDescription depth_attachment_desc;
    __cvk::get_default_attachment_description(VK_FORMAT_D16_UNORM, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL, depth_attachment_desc);
    cvk::RenderPass render_pass(device);
    render_pass.attaches(color_attachment_desc, depth_attachment_desc, subpass.get_description());
    VkResult result = render_pass.create();
    CHECK(result == VK_SUCCESS);

    std::vector<cvk::Framebuffer> framebuffers;
    auto CONST_REFERENCE images = swapchain.get_images();
    std::vector<cvk::ImageView2D> image_views2d;
    for (auto i : images) {
        CHECK(image_views2d.emplace_back(device, i).create_image_view(swapchain.info()) == VK_SUCCESS);
        framebuffers.emplace_back(device, render_pass, width, height).attaches((VkImageView)image_views2d.back(), (VkImageView)depth);
        CHECK(framebuffers.back().create() == VK_SUCCESS);
    }

    struct Vertex {
        float position[3];
        float color[3];
    };

    cvk::Shader vert_shader(device, utils::load_file("triangle.vert.spv"));
    CHECK(vert_shader.create() == VK_SUCCESS);
    cvk::Shader frag_shader(device, utils::load_file("triangle.frag.spv"));
    CHECK(frag_shader.create() == VK_SUCCESS);

    cvk::Descriptor descriptor(device);
    descriptor.add_layout()
        .set(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1);
    CHECK(descriptor.create() == VK_SUCCESS);

    cvk::PipelineLayout layout(device);
    layout.attaches(static_cast<VkDescriptorSetLayout>(descriptor.get_layout(0)));
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

    // Setup vertices
    std::vector<Vertex> vertex =
    {
        { {  1.0f,  1.0f, 0.5f }, { 1.0f, 0.0f, 0.0f } },
        { { -1.0f,  1.0f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
        { {  0.0f, -1.0f, 0.5f }, { 0.0f, 0.0f, 1.0f } }
    };
    uint32_t vertex_size = static_cast<uint32_t>(vertex.size()) * sizeof(Vertex);

    // Setup indices
    std::vector<uint32_t> index = { 0, 1, 2 };
    uint32_t indices_count = static_cast<uint32_t>(index.size());
    uint32_t index_size = indices_count * sizeof(uint32_t);

    std::vector<glm::mat4> ubo = {
        glm::lookAt(glm::vec3(-5, 3, -10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
        glm::perspective(glm::radians(60.f), static_cast<float>(width / height), 0.1f, 256.0f)
    };
    uint32_t ubo_size = sizeof(glm::mat4) * ubo.size();

    cvk::Buffer vertex_buffer(device, vertex_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    CHECK(vertex_buffer.create() == VK_SUCCESS);
    cvk::Memory vertex_buffer_mem(device, device.get_memory_properties(), vertex_buffer.get_memory_requirement(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    CHECK(vertex_buffer_mem.allocate() == VK_SUCCESS);
    CHECK(vertex_buffer_mem.bind(vertex_buffer) == VK_SUCCESS);
    char* ptr = nullptr;
    CHECK(vertex_buffer_mem.map(ptr) == VK_SUCCESS);
    memcpy(ptr, vertex.data(), vertex_size);
    vertex_buffer_mem.unmap();

    cvk::Buffer uniform_buffer(device, ubo_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    CHECK(uniform_buffer.create() == VK_SUCCESS);
    cvk::Memory uniform_buffer_mem(device, device.get_memory_properties(), uniform_buffer.get_memory_requirement(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    CHECK(uniform_buffer_mem.allocate() == VK_SUCCESS);
    CHECK(uniform_buffer_mem.bind(uniform_buffer) == VK_SUCCESS);
    CHECK(uniform_buffer_mem.map(ptr) == VK_SUCCESS);
    memcpy(ptr, ubo.data(), ubo_size);
    uniform_buffer_mem.unmap();

    VkDescriptorBufferInfo ubo_copy_info;
    __cvk::get_default_descriptor_buffer_info(ubo_size, uniform_buffer, ubo_copy_info);
    cvk::WriteDescriptorSet ubo_copy_set(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0);
    ubo_copy_set.attaches(ubo_copy_info);
    ubo_copy_set.update(device, descriptor[0]);

    cvk::CommandPool command_pool(device, graphics_index);
    CHECK(command_pool.create() == VK_SUCCESS);

    cvk::CommandBuffer command_buffer(device, command_pool);
    CHECK(command_buffer.create() == VK_SUCCESS);

    cvk::Semaphore acquire_semaphore(device);
    CHECK(acquire_semaphore.create() == VK_SUCCESS);
    cvk::Fence wait_fence(device);
    CHECK(wait_fence.create() == VK_SUCCESS);

    std::vector<VkClearValue> clear_values(2);
    clear_values[0].color.float32[0] = 0.1f;
    clear_values[0].color.float32[1] = 0.2f;
    clear_values[0].color.float32[2] = 0.4f;
    clear_values[0].color.float32[3] = 0.2f;
    clear_values[1].depthStencil.depth = 1.0f;
    clear_values[1].depthStencil.stencil = 0;
    VkRect2D render_area = {{0, 0}, {width, height}};

    VkViewport viewport = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
    VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    cvk::Queue graphics_queue(device, graphics_index);

    win.run([&]{ 
        uint32_t cur_index = swapchain.acquire(acquire_semaphore);

        CHECK(command_buffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) == VK_SUCCESS);

        VkRenderPassBeginInfo begin_renderpass_info;
        __cvk::get_default_begin_renderpass_info(render_pass, framebuffers[cur_index], clear_values, render_area, begin_renderpass_info);
        command_buffer.cmd().begin_renderpass(begin_renderpass_info);

        command_buffer.cmd().bind_pipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
        command_buffer.cmd().bind_descriptor_sets(VK_PIPELINE_BIND_POINT_GRAPHICS, layout, { descriptor[0] });
        command_buffer.cmd().bind_vertex_buffers({ vertex_buffer });
        command_buffer.cmd().set_viewport({ viewport });
        command_buffer.cmd().set_scissor({ render_area });
        command_buffer.cmd().draw(3);

        command_buffer.cmd().end_renderpass();
        command_buffer.end();

        graphics_queue
            .set_wait(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, { acquire_semaphore })
            .submit({ command_buffer }, wait_fence);
        CHECK(wait_fence.wait() == VK_SUCCESS);
        CHECK(wait_fence.reset() == VK_SUCCESS);
        // CHECK(vkResetFences(device, 1, &(VkFence CONST_REFERENCE)wait_fence) == VK_SUCCESS);

        swapchain.present(graphics_queue, {});

    });

TEST_FUNC_END