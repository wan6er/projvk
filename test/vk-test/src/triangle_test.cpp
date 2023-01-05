#include "test_header.h"

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/memory.h"
#include "cvk/swapchain.h"
#include "cvk/shader.h"
#include "cvk/buffer.h"
#include "cvk/framebuffer.h"

#include "cvk/sync/sync_set.h"

#include "cvk/command/command_pool.h"
#include "cvk/command/command_buffer.h"

#include "cvk/image/image_view.h"
#include "cvk/image/sampler.h"

#include "cvk/pipe/descriptor_set_layout.h"
#include "cvk/pipe/descriptor_set.h"
#include "cvk/pipe/descriptor_pool.h"
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
    cvk::Swapchain swapchain(device.get_physical_device(), surface, { VK_PRESENT_MODE_FIFO_KHR }, formats[0]);
    swapchain.create(device);

    uint32_t graphics_index = UINT32_MAX, present_index = UINT32_MAX;
    __cvk::get_queue_family_index(device.get_physical_device(), VK_QUEUE_GRAPHICS_BIT, graphics_index);
    __cvk::get_queue_family_index_present_support(device.get_physical_device(), surface, VK_QUEUE_GRAPHICS_BIT, present_index);
    CHECK(graphics_index != UINT32_MAX);
    CHECK(present_index != UINT32_MAX);

    
    cvk::ImageView2D depth(VK_IMAGE_ASPECT_DEPTH_BIT, VK_FORMAT_D16_UNORM, width, height, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL);
    depth.image_info().initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
    CHECK(depth.create_image(device) == VK_SUCCESS);
    cvk::Memory depth_mem(device.get_memory_properties(), depth.get_memory_requirement(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    CHECK(depth_mem.allocate(device) == VK_SUCCESS);
    depth_mem.bind(depth);
    CHECK(depth.create_image_view(device) == VK_SUCCESS);

    cvk::Sampler sampler;
    sampler.create(device);

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
    VkResult result = render_pass.create(device);
    CHECK(result == VK_SUCCESS);

    std::vector<cvk::Framebuffer> framebuffers;
    auto CONST_REFERENCE images = swapchain.get_images();
    std::vector<cvk::ImageView2D> image_views2d;
    for (auto i : images) {
        CHECK(image_views2d.emplace_back(swapchain.info(), i).create_image_view(device) == VK_SUCCESS);
        framebuffers.emplace_back(render_pass, width, height).attaches((VkImageView)image_views2d.back(), (VkImageView)depth);
        CHECK(framebuffers.back().create(device) == VK_SUCCESS);
    }

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
    descriptor_layout.attaches(binding0);
    CHECK(descriptor_layout.create(device) == VK_SUCCESS);

    cvk::DescriptorPool descriptor_pool(swapchain.get_images().size());
    descriptor_pool.set(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1);
    result = descriptor_pool.create(device);
    CHECK(result == VK_SUCCESS);

    cvk::DescriptorSet ubo_set(descriptor_pool, descriptor_layout);
    result = ubo_set.allocate(device);
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

    cvk::Buffer vertex_buffer(vertex_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    CHECK(vertex_buffer.create(device) == VK_SUCCESS);
    cvk::Memory vertex_buffer_mem(device.get_memory_properties(), vertex_buffer.get_memory_requirement(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    CHECK(vertex_buffer_mem.allocate(device) == VK_SUCCESS);
    CHECK(vertex_buffer_mem.bind(vertex_buffer) == VK_SUCCESS);
    char* ptr = nullptr;
    CHECK(vertex_buffer_mem.map(ptr) == VK_SUCCESS);
    memcpy(ptr, vertex.data(), vertex_size);
    vertex_buffer_mem.unmap();

    cvk::Buffer uniform_buffer(ubo_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    CHECK(uniform_buffer.create(device) == VK_SUCCESS);
    cvk::Memory uniform_buffer_mem(device.get_memory_properties(), uniform_buffer.get_memory_requirement(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    CHECK(uniform_buffer_mem.allocate(device) == VK_SUCCESS);
    CHECK(uniform_buffer_mem.bind(uniform_buffer) == VK_SUCCESS);
    CHECK(uniform_buffer_mem.map(ptr) == VK_SUCCESS);
    memcpy(ptr, ubo.data(), ubo_size);
    uniform_buffer_mem.unmap();

    VkDescriptorBufferInfo ubo_copy_info;
    __cvk::get_default_descriptor_buffer_info(ubo_size, uniform_buffer, ubo_copy_info);
    cvk::WriteDescriptorSet ubo_copy_set(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0);
    ubo_copy_set.attaches(ubo_copy_info);
    ubo_copy_set.update(device, ubo_set);

    cvk::CommandPool command_pool(graphics_index);
    CHECK(command_pool.create(device) == VK_SUCCESS);
    // std::vector<cvk::CommandBuffer> command_buffer(swapchain.get_images().size());
    // for (auto& cb : command_buffer) {
    //     CHECK(cb.create(device, command_pool) == VK_SUCCESS);
    // }
    
    cvk::CommandBuffer command_buffer;
    CHECK(command_buffer.create(device, command_pool) == VK_SUCCESS);

    cvk::Semaphore acquire_semaphore;
    CHECK(acquire_semaphore.create(device) == VK_SUCCESS);
    cvk::Fence wait_fence;
    CHECK(wait_fence.create(device) == VK_SUCCESS);

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

    VkQueue graphics_queue;
    vkGetDeviceQueue(device, graphics_index, 0, &graphics_queue);
    CHECK(graphics_queue != VK_NULL_HANDLE);

    // win.run([&]{ 
    //     uint32_t cur_index = 0;
    //     vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, acquire_semaphore, VK_NULL_HANDLE, &cur_index);

    //     CHECK(command_buffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) == VK_SUCCESS);

    //     VkRenderPassBeginInfo begin_renderpass_info;
    //     __cvk::get_default_begin_renderpass_info(render_pass, framebuffers[cur_index], clear_values, render_area, begin_renderpass_info);
    //     command_buffer.cmd().begin_renderpass(begin_renderpass_info);

    //     command_buffer.cmd().bind_pipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    //     command_buffer.cmd().bind_descriptor_sets(VK_PIPELINE_BIND_POINT_GRAPHICS, layout, { ubo_set });
    //     command_buffer.cmd().bind_vertex_buffers({ vertex_buffer });
    //     command_buffer.cmd().set_viewport({ viewport });
    //     command_buffer.cmd().set_scissor({ render_area });
    //     command_buffer.cmd().draw(3);

    //     command_buffer.cmd().end_renderpass();
    //     command_buffer.end();

    //     VkSubmitInfo submit_info[1] = {};
    //     submit_info[0].pNext = NULL;
    //     submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    //     submit_info[0].waitSemaphoreCount = 1;
    //     submit_info[0].pWaitSemaphores = &(VkSemaphore CONST_REFERENCE)acquire_semaphore;
    //     submit_info[0].pWaitDstStageMask = &pipe_stage_flags;
    //     submit_info[0].commandBufferCount = 1;
    //     submit_info[0].pCommandBuffers = &(VkCommandBuffer CONST_REFERENCE)command_buffer;
    //     submit_info[0].signalSemaphoreCount = 0;
    //     submit_info[0].pSignalSemaphores = NULL;
    //     /* Queue the command buffer for execution */
    //     CHECK(vkQueueSubmit(graphics_queue, 1, submit_info, wait_fence) == VK_SUCCESS);
    //     CHECK(vkWaitForFences(device, 1, &(VkFence CONST_REFERENCE)wait_fence, VK_TRUE, UINT64_MAX) == VK_SUCCESS);
    //     CHECK(vkResetFences(device, 1, &(VkFence CONST_REFERENCE)wait_fence) == VK_SUCCESS);
            
    //     VkPresentInfoKHR present;
    //     present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    //     present.pNext = NULL;
    //     present.swapchainCount = 1;
    //     present.pSwapchains = &(VkSwapchainKHR CONST_REFERENCE)swapchain;
    //     present.pImageIndices = &cur_index;
    //     present.pWaitSemaphores = NULL;
    //     present.waitSemaphoreCount = 0;
    //     present.pResults = NULL;
    //     CHECK(vkQueuePresentKHR(graphics_queue, &present) == VK_SUCCESS);

    // });

TEST_FUNC_END