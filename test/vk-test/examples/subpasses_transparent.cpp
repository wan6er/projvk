#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/memory.h"
#include "cvk/swapchain.h"
#include "cvk/shader.h"
#include "cvk/buffer.h"
#include "cvk/framebuffer.h"
#include "cvk/descriptor.h"
#include "cvk/render_pass.h"
#include "cvk/graphics_pipeline.h"
#include "cvk/image.h"
#include "cvk/memorized_buffer.h"
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

#include "load_texture.h"
#include "load_model.h"


#ifdef WIN32
#include "win32/surface_win32.h"
#include "win32/win.h"
#endif

int main()
{

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
    cvk::Device device(devices[0], device_extensions, device_features, VK_QUEUE_GRAPHICS_BIT);

    uint32_t width = 1024;
    uint32_t height = 720;
#ifdef WIN32
    Windows win("model", width, height);
    cvk::SurfaceWin32 surface(instance, win.instance(), win);
#else
#error unsupport platform
#endif

    std::vector<VkClearValue> clear_values(8);
    clear_values[7].depthStencil.depth = 1.0f;
    clear_values[7].depthStencil.stencil = 0;

    VkRect2D render_area = {{0, 0}, {width, height}};
    VkViewport viewport = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };

    std::vector<VkSurfaceFormatKHR> formats;
    __cvk::get_surface_formats(device.get_physical_device(), surface, formats);
    CVK_ASSERT(formats.size() > 0);
    cvk::Swapchain swapchain(device, device.get_physical_device(), surface, { VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR }, formats[0]);
    swapchain.create();

    uint32_t graphics_index = UINT32_MAX, present_index = UINT32_MAX;
    __cvk::get_queue_family_index(device.get_physical_device(), VK_QUEUE_GRAPHICS_BIT, graphics_index);
    __cvk::get_queue_family_index_present_support(device.get_physical_device(), surface, VK_QUEUE_GRAPHICS_BIT, present_index);
    CVK_ASSERT(graphics_index != UINT32_MAX);
    CVK_ASSERT(present_index != UINT32_MAX);

    cvk::StandardInputAttachment2D position_attachment(device);
    CVK_ASSERT(position_attachment.create(device.get_memory_properties(), VK_FORMAT_R16G16B16A16_SFLOAT, width, height) == VK_SUCCESS);
    cvk::StandardInputAttachment2D normal_attachment(device);
    CVK_ASSERT(normal_attachment.create(device.get_memory_properties(), VK_FORMAT_R16G16B16A16_SFLOAT, width, height) == VK_SUCCESS);
    cvk::StandardInputAttachment2D albedo_attachment(device);
    CVK_ASSERT(albedo_attachment.create(device.get_memory_properties(), VK_FORMAT_R8G8B8A8_UNORM, width, height) == VK_SUCCESS);

    cvk::StandardInputAttachment2D transparent_position_attachment(device);
    CVK_ASSERT(transparent_position_attachment.create(device.get_memory_properties(), VK_FORMAT_R16G16B16A16_SFLOAT, width, height) == VK_SUCCESS);
    cvk::StandardInputAttachment2D transparent_normal_attachment(device);
    CVK_ASSERT(transparent_normal_attachment.create(device.get_memory_properties(), VK_FORMAT_R16G16B16A16_SFLOAT, width, height) == VK_SUCCESS);
    cvk::StandardInputAttachment2D transparent_albedo_attachment(device);
    CVK_ASSERT(transparent_albedo_attachment.create(device.get_memory_properties(), VK_FORMAT_R8G8B8A8_UNORM, width, height) == VK_SUCCESS);

    cvk::StandardDepthAttachment2D depth(device);
    CVK_ASSERT(depth.create(device.get_memory_properties(), VK_FORMAT_D16_UNORM, width, height) == VK_SUCCESS);

    cvk::Sampler sampler(device);
    sampler.create();

    cvk::RenderPass render_pass(device);
    render_pass.add_subpass(VK_PIPELINE_BIND_POINT_GRAPHICS)
        .add_color(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .add_color(1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .add_color(2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .set_depth(7);
    render_pass.add_subpass(VK_PIPELINE_BIND_POINT_GRAPHICS)
        .add_color(3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .add_color(4, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .add_color(5, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .set_depth(7);
    render_pass.add_subpass(VK_PIPELINE_BIND_POINT_GRAPHICS)
        .add_input(0, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        .add_input(1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        .add_input(2, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)

        .add_input(3, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        .add_input(4, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        .add_input(5, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)

        .add_color(6, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .set_depth(7);
    render_pass.add_subpass_dependency(0, 1)
        .set_src(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)
        .set_dst(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    render_pass.add_subpass_dependency(1, 2)
        .set_src(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)
        .set_dst(VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_SHADER_READ_BIT);
    render_pass
        .add_attachment(VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .add_attachment(VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .add_attachment(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)

        .add_attachment(VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .add_attachment(VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .add_attachment(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)

        .add_attachment(VK_FORMAT_B8G8R8A8_UNORM, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
        .add_attachment(VK_FORMAT_D16_UNORM, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    CVK_ASSERT(render_pass.create() == VK_SUCCESS);

    std::vector<cvk::Framebuffer> framebuffers;
    auto CONST_REFERENCE images = swapchain.get_images();
    std::vector<cvk::ColorImageView2D> image_views2d;
    for (auto i : images) {
        CVK_ASSERT(image_views2d.emplace_back(device).create(swapchain.get_format(), i) == VK_SUCCESS);
        framebuffers.emplace_back(device, render_pass, width, height).attaches(
            (VkImageView)position_attachment,
            (VkImageView)normal_attachment,
            (VkImageView)albedo_attachment,
            (VkImageView)transparent_position_attachment,
            (VkImageView)transparent_normal_attachment,
            (VkImageView)transparent_albedo_attachment,
            (VkImageView)image_views2d.back(),
            (VkImageView)depth);
        CVK_ASSERT(framebuffers.back().create() == VK_SUCCESS);
    }

    glm::vec3 view_pos = glm::vec3(0, 20, -20);
    std::vector<glm::mat4> ubo = {
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0)),
        glm::lookAt(view_pos, glm::vec3(0, 0, 0), glm::vec3(0, -1, 0)),
        glm::perspective(glm::radians(60.f), static_cast<float>(width / height), 0.1f, 100.0f)
    };
    uint32_t ubo_size = sizeof(glm::mat4) * ubo.size();

    cvk::WritableUniformBuffer uniform_buffer(device);
    CVK_ASSERT(uniform_buffer.create(device.get_memory_properties(), ubo_size) == VK_SUCCESS);
    CVK_ASSERT(uniform_buffer.upload(ubo.data(), ubo_size) == VK_SUCCESS);

    cvk::StandardTexture2D texture(device);
    load_texture(device, "texture/colored_glass_rgba.ktx", graphics_index, texture);

    std::vector<NodeBuffers> building_buffers;
    load_vertex(device, "model/samplebuilding.gltf", building_buffers);
    std::vector<NodeBuffers> building_glass_buffers;
    load_vertex(device, "model/samplebuilding_glass.gltf", building_glass_buffers);
    // load_vertex(device, "model/retroufo_glow.gltf", building_buffers);
    
    cvk::Descriptor descriptor(device);
    for (auto& buf : building_buffers) {
        descriptor.add_layout()
            .set(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1)
            .set(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1)
            .set(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
    }
    CVK_ASSERT(descriptor.create() == VK_SUCCESS);

    for (uint32_t i = 0; i < building_buffers.size(); ++i) {
        descriptor[i].write(0, uniform_buffer.get_descriptor_info());
        descriptor[i].write(1, building_buffers[i].transform.get_descriptor_info());
        descriptor[i].write(2, texture.get_descriptor_info(sampler, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
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

    cvk::GraphicsPipeline obj_pipeline(device, render_pass, layout);
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
    CVK_ASSERT(obj_pipeline.create() == VK_SUCCESS);

    cvk::Descriptor glass_descriptor(device);
    for (auto& buf : building_glass_buffers) {
        glass_descriptor.add_layout()
            .set(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1)
            .set(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1)
            .set(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
    }
    CVK_ASSERT(glass_descriptor.create() == VK_SUCCESS);
    for (uint32_t i = 0; i < building_glass_buffers.size(); ++i) {
        glass_descriptor[i].write(0, uniform_buffer.get_descriptor_info());
        glass_descriptor[i].write(1, building_glass_buffers[i].transform.get_descriptor_info());
        glass_descriptor[i].write(2, texture.get_descriptor_info(sampler, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
    }

    cvk::GraphicsPipeline transparent_pipeline = obj_pipeline;
    cvk::PipelineLayout transparent_layout(device);
    transparent_layout.attaches(static_cast<VkDescriptorSetLayout>(glass_descriptor[0].get_layout()));
    CVK_ASSERT(transparent_layout.create() == VK_SUCCESS);
    {
        cvk::GraphicsPipeline _temp(device, render_pass, transparent_layout);
        obj_pipeline.count_swap(_temp);
    }
    // transparent_pipeline.rasterization().polygonMode = VK_POLYGON_MODE_LINE;
    transparent_pipeline.set_subpass(1);
    transparent_pipeline.shader().clear_all();
    transparent_pipeline.shader()
        .attach(VK_SHADER_STAGE_VERTEX_BIT, vert_shader)
        .attach(VK_SHADER_STAGE_FRAGMENT_BIT, gbufft_frag_shader);
    CVK_ASSERT(transparent_pipeline.create() == VK_SUCCESS);

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
    cvk::WritableUniformBuffer light_ubo_buffer(device);
    CVK_ASSERT(light_ubo_buffer.create(device.get_memory_properties(), sizeof(CompositionUBO)) == VK_SUCCESS);
    CVK_ASSERT(light_ubo_buffer.upload(&light_ubo, sizeof(light_ubo)) == VK_SUCCESS);

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
    
    light_descriptor[0].write(0, position_attachment.get_descriptor_info(VK_NULL_HANDLE, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
    light_descriptor[0].write(1, normal_attachment.get_descriptor_info(VK_NULL_HANDLE, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
    light_descriptor[0].write(2, albedo_attachment.get_descriptor_info(VK_NULL_HANDLE, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
    
    light_descriptor[0].write(3, transparent_position_attachment.get_descriptor_info(VK_NULL_HANDLE, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
    light_descriptor[0].write(4, transparent_normal_attachment.get_descriptor_info(VK_NULL_HANDLE, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
    light_descriptor[0].write(5, transparent_albedo_attachment.get_descriptor_info(VK_NULL_HANDLE, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));

    light_descriptor[0].write(6, light_ubo_buffer.get_descriptor_info());

    cvk::PipelineLayout light_layout(device);
    light_layout.attaches(static_cast<VkDescriptorSetLayout>(light_descriptor[0].get_layout()));
    CVK_ASSERT(light_layout.create() == VK_SUCCESS);

    cvk::Shader composition_vert_shader(device, utils::load_file("shader/composition.vert.spv"));
    CVK_ASSERT(composition_vert_shader.create() == VK_SUCCESS);
    cvk::Shader composition_frag_shader(device, utils::load_file("shader/composition_transparent.frag.spv"));
    CVK_ASSERT(composition_frag_shader.create() == VK_SUCCESS);

    cvk::GraphicsPipeline light_pipeline(device, render_pass, light_layout);
    light_pipeline.viewport().attaches(render_area, viewport);
    light_pipeline.color_blend()
        .attach(0xf, false);
    light_pipeline.set_subpass(2);
    light_pipeline.shader()
        .attach(VK_SHADER_STAGE_VERTEX_BIT, composition_vert_shader)
        .attach(VK_SHADER_STAGE_FRAGMENT_BIT, composition_frag_shader);
    CVK_ASSERT(light_pipeline.create() == VK_SUCCESS);

    cvk::CommandPool command_pool(device, graphics_index);
    CVK_ASSERT(command_pool.create() == VK_SUCCESS);

    cvk::CommandBuffer command_buffer(device, command_pool);
    CVK_ASSERT(command_buffer.create() == VK_SUCCESS);

    cvk::Semaphore acquire_semaphore(device);
    CVK_ASSERT(acquire_semaphore.create() == VK_SUCCESS);
    cvk::Fence wait_fence(device);
    CVK_ASSERT(wait_fence.create() == VK_SUCCESS);

    cvk::Queue graphics_queue(device, graphics_index);

    utils::Stopwatch benchmark;
    benchmark.start();
    uint32_t time_i = 0;
    uint32_t time = 0;

    bool should_close = false;
    uint32_t msg = 0;
    win.show();
    while (!should_close) {

        uint32_t cur_index = swapchain.acquire(acquire_semaphore);

        CVK_ASSERT(command_buffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) == VK_SUCCESS);

        VkRenderPassBeginInfo begin_renderpass_info;
        __cvk::get_default_begin_renderpass_info(render_pass, framebuffers[cur_index], clear_values, render_area, begin_renderpass_info);
        command_buffer.cmd().begin_renderpass(begin_renderpass_info);

        command_buffer.cmd().bind_pipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, obj_pipeline);
        for (uint32_t i = 0; i < building_buffers.size(); ++i) 
        {
            command_buffer.cmd().bind_descriptor_sets(VK_PIPELINE_BIND_POINT_GRAPHICS, layout, { descriptor[i] });
            command_buffer.cmd().bind_vertex_buffer(building_buffers[i].vertex);
            command_buffer.cmd().bind_index_buffer(VK_INDEX_TYPE_UINT16, building_buffers[i].index);
            command_buffer.cmd().draw_indexed(building_buffers[i].index.get_size() / sizeof(uint16_t));
        }

        command_buffer.cmd().next_subpass();

        command_buffer.cmd().bind_pipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, transparent_pipeline);
        for (uint32_t i = 0; i < building_glass_buffers.size(); ++i) 
        {
            command_buffer.cmd().bind_descriptor_sets(VK_PIPELINE_BIND_POINT_GRAPHICS, transparent_layout, { glass_descriptor[i] });
            command_buffer.cmd().bind_vertex_buffer(building_glass_buffers[i].vertex);
            command_buffer.cmd().bind_index_buffer(VK_INDEX_TYPE_UINT16, building_glass_buffers[i].index);
            command_buffer.cmd().draw_indexed(building_glass_buffers[i].index.get_size() / sizeof(uint16_t));
        }

        command_buffer.cmd().next_subpass();
        
        {
            command_buffer.cmd().bind_pipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, light_pipeline);
            command_buffer.cmd().bind_descriptor_sets(VK_PIPELINE_BIND_POINT_GRAPHICS, light_layout, { light_descriptor[0] });
            command_buffer.cmd().draw(6);
        }

        command_buffer.cmd().end_renderpass();
        command_buffer.end();

        graphics_queue
            .set_wait(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, { acquire_semaphore })
            .submit({ command_buffer }, wait_fence);

        CVK_ASSERT(wait_fence.wait() == VK_SUCCESS);
        CVK_ASSERT(wait_fence.reset() == VK_SUCCESS);

        swapchain.present(graphics_queue, {});

        time += benchmark.lap();
        if (time_i++ == 5000) {
            std::cout << "fps : " << (1000.0f * 5000.0f) / time << "\n";
            time = 0;
            time_i = 0;
        }

        if (win.poll_event(msg)) {
            switch (msg)
            {
            case WM_QUIT:
                should_close = true;
                break;
            }
            win.update();
        }

    }
}