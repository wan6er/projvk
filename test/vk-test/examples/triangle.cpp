#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/memory.h"
#include "cvk/swapchain.h"
#include "cvk/shader.h"
#include "cvk/buffer.h"
#include "cvk/framebuffer.h"
#include "cvk/image.h"

#include "cvk/sync/sync_set.h"

#include "cvk/command/queue.h"
#include "cvk/command_pool.h"
#include "cvk/command_buffer.h"

#include "cvk/image/image_view.h"
#include "cvk/image/sampler.h"

// #include "cvk/descriptor/descriptor_set_layout.h"
// #include "cvk/descriptor/descriptor_set.h"
#include "cvk/descriptor/write_descriptor_set.h"
#include "cvk/descriptor.h"
#include "cvk/render_pass.h"
#include "cvk/graphics_pipeline.h"
#include "cvk/pipe/pipeline_layout.h"
#include "cvk/pipe/subpass.h"

#include "cvk/initialize/command_initialize.h"
#include "cvk/initialize/memory_initialize.h"
#include "cvk/initialize/swapchain_initialize.h"
#include "cvk/initialize/surface_initialize.h"
#include "cvk/initialize/descriptor_initialize.h"
#include "cvk/initialize/pipe_initialize.h"

#include "utils/file.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#ifdef WIN32
#include "win32/surface_win32.h"
#include "win32/win.h"
#elif linux
#include "linux/win_xcb.h"
#include "linux/surface_xcb.h"
#endif

int main()
{

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

    std::cout << "init device\n";

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
    std::cout << "init window\n";

    std::vector<VkSurfaceFormatKHR> formats;
    __cvk::get_surface_formats(device.get_physical_device(), surface, formats);
    CVK_ASSERT(formats.size() > 0);
    auto format = formats[0].format;
    cvk::Swapchain swapchain(device, device.get_physical_device(), surface, { VK_PRESENT_MODE_FIFO_KHR }, formats[0]);
    swapchain.create();
    std::cout << "init swapchain\n";

    uint32_t graphics_index = UINT32_MAX, present_index = UINT32_MAX;
    __cvk::get_queue_family_index(device.get_physical_device(), VK_QUEUE_GRAPHICS_BIT, graphics_index);
    __cvk::get_queue_family_index_present_support(device.get_physical_device(), surface, VK_QUEUE_GRAPHICS_BIT, present_index);
    CVK_ASSERT(graphics_index != UINT32_MAX);
    CVK_ASSERT(present_index != UINT32_MAX);

    cvk::StandardDepthAttachment2D depth(device);
    CVK_ASSERT(depth.create(device.get_memory_properties(), VK_FORMAT_D16_UNORM, width, height) == VK_SUCCESS);

    cvk::Sampler sampler(device);
    sampler.create();

    cvk::RenderPass render_pass(device);
    render_pass.add_subpass(VK_PIPELINE_BIND_POINT_GRAPHICS)
        .add_color(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .set_depth(1);
    render_pass
        .add_attachment(format, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
        .add_attachment(VK_FORMAT_D16_UNORM, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    CVK_ASSERT(render_pass.create() == VK_SUCCESS);
    std::cout << "init renderpass\n";

    std::vector<cvk::Framebuffer> framebuffers;
    auto CONST_REFERENCE images = swapchain.get_images();
    std::cout << "images count " << images.size() << "\n";

    std::vector<cvk::ColorImageView2D> image_views2d;
    for (auto i : images) {
        CVK_ASSERT(image_views2d.emplace_back(device).create(swapchain.get_format(), i) == VK_SUCCESS);
        framebuffers.emplace_back(device, render_pass, width, height).attaches((VkImageView)image_views2d.back(), (VkImageView)depth);
        CVK_ASSERT(framebuffers.back().create() == VK_SUCCESS);
    }
    std::cout << "init framebuffer\n";

    struct Vertex {
        float position[3];
        float color[3];
    };

    cvk::Shader vert_shader(device, utils::load_file("bin/shader/triangle.vert.spv"));
    CVK_ASSERT(vert_shader.create() == VK_SUCCESS);
    cvk::Shader frag_shader(device, utils::load_file("bin/shader/triangle.frag.spv"));
    CVK_ASSERT(frag_shader.create() == VK_SUCCESS);

    std::cout << "init shader\n";

    cvk::Descriptor descriptor(device);
    descriptor.add_layout()
        .set(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1);
    CVK_ASSERT(descriptor.create() == VK_SUCCESS);

    cvk::PipelineLayout layout(device);
    layout.attaches(descriptor[0].get_layout());
    CVK_ASSERT(layout.create() == VK_SUCCESS);

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
    CVK_ASSERT(pipeline.create() == VK_SUCCESS);

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

    cvk::Buffer vertex_buffer(device);
    CVK_ASSERT(vertex_buffer.create(vertex_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) == VK_SUCCESS);
    cvk::Memory vertex_buffer_mem(device);
    CVK_ASSERT(vertex_buffer_mem.allocate(device.get_memory_properties(), vertex_buffer.get_memory_requirement(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == VK_SUCCESS);
    CVK_ASSERT(vertex_buffer_mem.bind(vertex_buffer) == VK_SUCCESS);
    char* ptr = nullptr;
    CVK_ASSERT(vertex_buffer_mem.map(ptr) == VK_SUCCESS);
    memcpy(ptr, vertex.data(), vertex_size);
    vertex_buffer_mem.unmap();

    cvk::Buffer uniform_buffer(device);
    CVK_ASSERT(uniform_buffer.create(ubo_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) == VK_SUCCESS);
    cvk::Memory uniform_buffer_mem(device);
    CVK_ASSERT(uniform_buffer_mem.allocate(device.get_memory_properties(), uniform_buffer.get_memory_requirement(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == VK_SUCCESS);
    CVK_ASSERT(uniform_buffer_mem.bind(uniform_buffer) == VK_SUCCESS);
    CVK_ASSERT(uniform_buffer_mem.map(ptr) == VK_SUCCESS);
    memcpy(ptr, ubo.data(), ubo_size);
    uniform_buffer_mem.unmap();

    VkDescriptorBufferInfo ubo_copy_info;
    __cvk::get_default_descriptor_buffer_info(ubo_size, uniform_buffer, ubo_copy_info);
    cvk::WriteDescriptorSet ubo_copy_set;
    ubo_copy_set.attaches(ubo_copy_info);
    ubo_copy_set.setup(descriptor[0], VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0).update(device);

    cvk::CommandPool command_pool(device, graphics_index);
    CVK_ASSERT(command_pool.create() == VK_SUCCESS);

    cvk::CommandBuffer command_buffer(device, command_pool);
    CVK_ASSERT(command_buffer.create() == VK_SUCCESS);

    cvk::Semaphore acquire_semaphore(device);
    CVK_ASSERT(acquire_semaphore.create() == VK_SUCCESS);
    cvk::Fence wait_fence(device);
    CVK_ASSERT(wait_fence.create() == VK_SUCCESS);

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

    std::cout << "prepare finish\n";
    uint32_t event = 0;
    while (win.poll_event(event)) {

        uint32_t cur_index = swapchain.acquire(acquire_semaphore);

        CVK_ASSERT(command_buffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) == VK_SUCCESS);

        VkRenderPassBeginInfo begin_renderpass_info;
        __cvk::get_default_begin_renderpass_info(render_pass, framebuffers[cur_index], clear_values, render_area, begin_renderpass_info);
        command_buffer.cmd().begin_renderpass(begin_renderpass_info);

        command_buffer.cmd().bind_pipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
        command_buffer.cmd().bind_descriptor_sets(VK_PIPELINE_BIND_POINT_GRAPHICS, layout, { descriptor[0] });
        command_buffer.cmd().bind_vertex_buffer(vertex_buffer);
        command_buffer.cmd().set_viewport({ viewport });
        command_buffer.cmd().set_scissor({ render_area });
        command_buffer.cmd().draw(3);

        command_buffer.cmd().end_renderpass();
        command_buffer.end();

        graphics_queue
            .set_wait(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, { acquire_semaphore })
            .submit({ command_buffer }, wait_fence);
        CVK_ASSERT(wait_fence.wait() == VK_SUCCESS);
        CVK_ASSERT(wait_fence.reset() == VK_SUCCESS);
        CVK_ASSERT(vkResetFences(device, 1, &(VkFence CONST_REFERENCE)wait_fence) == VK_SUCCESS);

        swapchain.present(graphics_queue, {});

        win.free_event();
    }
}