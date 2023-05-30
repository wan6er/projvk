#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/memory.h"
#include "cvk/swapchain.h"
#include "cvk/shader.h"
#include "cvk/buffer.h"
#include "cvk/descriptor.h"
#include "cvk/command_pool.h"
#include "cvk/command_buffer.h"
#include "cvk/acceleration_structure.h"
#include "cvk/fence.h"
#include "cvk/pipeline_raytracing.h"
#include "cvk/image.h"
#include "cvk/framebuffer.h"

#include "cvk/initialize/surface_initialize.h"

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


class AccStructTriangle
{
private:
    cvk::Device _device;
    cvk::CommandPool _cmdpool;
    cvk::Queue _queue;

    cvk::AccelerationStructureInputBufferWritable vertex_buffer;
    cvk::AccelerationStructureInputBufferWritable index_buffer;
    cvk::AccelerationStructureInputBufferWritable bottom_transform_buffer;
    cvk::AccelerationStructureBuffer bottom_buffer;
    cvk::AccelerationStructure bottom;

    cvk::AccelerationStructureInputBufferWritable top_transform_buffer;
    cvk::AccelerationStructureBuffer top_buffer;
    cvk::AccelerationStructure top;

    cvk::Descriptor descriptor;

    cvk::PipelineLayout layout;
    cvk::RayTracingPipeline pipeline;

    cvk::UniformBufferWritable uniform_buffer;
    cvk::StorageImage2D storage_image;
    cvk::ShaderBindingTableBufferWritable rgen_sbt_buffer;
    cvk::ShaderBindingTableBufferWritable rmiss_sbt_buffer;
    cvk::ShaderBindingTableBufferWritable rchit_sbt_buffer;

    size_t cmdbufs_size;
    std::vector<cvk::CommandBuffer> cmdbufs;
    cvk::CommandBuffer cmdbuf;

public:
    AccStructTriangle(cvk::Device device, cvk::CommandPool cmdpool, cvk::Queue queue, size_t cmdsize) :
        _device(device), _cmdpool(cmdpool), _queue(queue),
        vertex_buffer(device), index_buffer(device), bottom_transform_buffer(device),
        bottom_buffer(device), bottom(device),
        top_transform_buffer(device), top_buffer(device), top(device),
        descriptor(device), layout(device), pipeline(device),
        uniform_buffer(device), storage_image(device), rgen_sbt_buffer(device), rmiss_sbt_buffer(device), rchit_sbt_buffer(device), 
        cmdbufs_size(cmdsize), cmdbuf(device, cmdpool)
    {
    }

    void create_bottom_level()
    {
        std::cout << "create bottom level\n";

        struct Vertex {
            float pos[3];
        };
        std::vector<Vertex> vertices = {
            { {  1.0f,  1.0f, 0.0f } },
            { { -1.0f,  1.0f, 0.0f } },
            { {  0.0f, -1.0f, 0.0f } }
        };

        std::vector<uint32_t> indices = { 0, 1, 2 };

        uint32_t indexCount = static_cast<uint32_t>(indices.size());
        VkTransformMatrixKHR transformMatrix = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f
        };

        CVK_ASSERT(vertex_buffer.create(_device.get_memory_properties(), sizeof(Vertex) * vertices.size()) == VK_SUCCESS);
        vertex_buffer.upload(vertices.data(), sizeof(Vertex) * vertices.size());
        CVK_ASSERT(index_buffer.create(_device.get_memory_properties(), sizeof(uint32_t) * indices.size()) == VK_SUCCESS);
        index_buffer.upload(indices.data(), sizeof(uint32_t) * indices.size());
        CVK_ASSERT(bottom_transform_buffer.create(_device.get_memory_properties(), sizeof(VkTransformMatrixKHR)) == VK_SUCCESS);
        bottom_transform_buffer.upload(&transformMatrix, sizeof(VkTransformMatrixKHR));

        cvk::AccStructGeometry geometry;
        auto& trigeometry = geometry.triangles();
        trigeometry.set_vertex_device_address(vertex_buffer.get_address());
        trigeometry.set_vertex_format(VK_FORMAT_R32G32B32_SFLOAT);
        trigeometry.set_vertex_max_num(3);
        trigeometry.set_vertex_stride(sizeof(Vertex));
        trigeometry.set_index_device_address(index_buffer.get_address());
        trigeometry.set_index_type(VK_INDEX_TYPE_UINT32);
        trigeometry.set_transform_device_address(bottom_transform_buffer.get_address());
        
        cvk::AccStructGeometries geometries;
        geometries.set_geometries_type(VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR);
        geometries.attach(geometry);

        auto geosize = geometries.get_geometries_size(_device, VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR);

        cvk::AccelerationStructureScratchBuffer scratch_bottom(_device);
        CVK_ASSERT(scratch_bottom.create(_device.get_memory_properties(), geosize.buildScratchSize) == VK_SUCCESS);

        CVK_ASSERT(bottom_buffer.create(_device.get_memory_properties(), geosize.accelerationStructureSize) == VK_SUCCESS);

        CVK_ASSERT(bottom.create(VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR, bottom_buffer, geosize.accelerationStructureSize) == VK_SUCCESS);

        cvk::CommandBuffer cmdbuffer(_device, _cmdpool);
        CVK_ASSERT(cmdbuffer.create() == VK_SUCCESS);

        auto& geoinfo = geometries.get_geometries_info();
        geoinfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
        geoinfo.dstAccelerationStructure = bottom;
        geoinfo.scratchData.deviceAddress = scratch_bottom.get_address();

        VkAccelerationStructureBuildRangeInfoKHR georange{};
        georange.primitiveCount = 3;

        cmdbuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        cmdbuffer.cmd().build_acceleration_structure(geometries.get_geometries_info(), georange);
        cmdbuffer.end();

        cvk::Fence fence(_device);
        fence.create();
        CVK_ASSERT(_queue.submit({ cmdbuffer }, fence) == VK_SUCCESS);

        fence.wait();

    }

    void create_top_level()
    {
        std::cout << "create top level\n";

        VkTransformMatrixKHR transformMatrix = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f };

        VkAccelerationStructureInstanceKHR instance{};
        instance.transform = transformMatrix;
        instance.instanceCustomIndex = 0;
        instance.mask = 0xFF;
        instance.instanceShaderBindingTableRecordOffset = 0;
        instance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
        instance.accelerationStructureReference = bottom.get_address();

        top_transform_buffer.create(_device.get_memory_properties(), sizeof(VkAccelerationStructureInstanceKHR));
        top_transform_buffer.upload(&instance, sizeof(VkAccelerationStructureInstanceKHR));

        cvk::AccStructGeometry geometry;
        auto& geoinstance = geometry.instances();
        geoinstance.set_device_address(top_transform_buffer.get_address());

        cvk::AccStructGeometries geometries;
        geometries.set_geometries_type(VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR);
        geometries.attach(geometry);
        auto geosize = geometries.get_geometries_size(_device, VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR);

        // cvk::AccelerationStructureBuffer ac_buffer(_device);
        top_buffer.create(_device.get_memory_properties(), geosize.accelerationStructureSize);
        cvk::AccelerationStructureScratchBuffer ac_scratch_buffer(_device);
        ac_scratch_buffer.create(_device.get_memory_properties(), geosize.buildScratchSize);

        // cvk::AccelerationStructure top_ac(_device);
        top.create(VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR, top_buffer, geosize.accelerationStructureSize);

        cvk::CommandBuffer cmdbuffer(_device, _cmdpool);
        cmdbuffer.create();

        auto& geoinfo = geometries.get_geometries_info();
        geoinfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
        geoinfo.dstAccelerationStructure = top;
        geoinfo.scratchData.deviceAddress = ac_scratch_buffer.get_address();

        VkAccelerationStructureBuildRangeInfoKHR georange{};
        georange.primitiveCount = 3;

        cmdbuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        cmdbuffer.cmd().build_acceleration_structure(geoinfo, georange);
        cmdbuffer.end();

        cvk::Fence fence(_device);
        fence.create();
        CVK_ASSERT(_queue.submit({ cmdbuffer }, fence) == VK_SUCCESS);

        fence.wait();
    }

    void create_raytracing_image(uint32_t width, uint32_t height)
    {
        std::cout << "create raytracing image\n";
        storage_image.create(_device.get_memory_properties(), VK_FORMAT_B8G8R8A8_UNORM, width, height);

        cvk::CommandBuffer transcmdbuf(_device, _cmdpool);
        transcmdbuf.create();
        transcmdbuf.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        transcmdbuf.cmd().set_image_layout_barrier(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL, storage_image, storage_image.get_subresource_range());
        transcmdbuf.end();

        cvk::Fence fence(_device);
        fence.create();
        CVK_ASSERT(_queue.submit({ transcmdbuf }, fence) == VK_SUCCESS);
        fence.wait();
    }

    void create_sbt()
    {
        auto properties = _device.get_raytracing_properties();
        auto aligned_size = cvk_aligned_size(properties.shaderGroupHandleSize, properties.shaderGroupHandleAlignment);

        std::cout << "create sbt\n";

        rgen_sbt_buffer.create(_device.get_memory_properties(), aligned_size);
        rmiss_sbt_buffer.create(_device.get_memory_properties(), aligned_size);
        rchit_sbt_buffer.create(_device.get_memory_properties(), aligned_size);
    }

    void create_descriptor(uint32_t width, uint32_t height)
    {    
        std::vector<glm::mat4> ubo = {
            glm::inverse(glm::lookAt(glm::vec3(-5, 3, -10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0))),
            glm::inverse(glm::perspective(glm::radians(60.f), static_cast<float>(width / height), 0.1f, 256.0f))
        };
        uint32_t ubo_size = sizeof(glm::mat4) * ubo.size();

        uniform_buffer.create(_device.get_memory_properties(), ubo_size);
        uniform_buffer.upload(ubo.data(), ubo_size);

        std::cout << "create descriptor\n";
        descriptor.add_layout()
            .set(0, VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, VK_SHADER_STAGE_RAYGEN_BIT_KHR, 1)
            .set(1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, VK_SHADER_STAGE_RAYGEN_BIT_KHR, 1)
            .set(2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_RAYGEN_BIT_KHR, 1);
        CVK_ASSERT(descriptor.create() == VK_SUCCESS);

        descriptor[0].write(0, top.get_object());
        descriptor[0].write(1, storage_image.get_descriptor_info(VK_IMAGE_LAYOUT_GENERAL));
        descriptor[0].write(2, uniform_buffer.get_descriptor_info());
    }

    void create_raytracing_pipeline()
    {
        std::cout << "create raytracing pipeline\n";

        cvk::Shader rgen(_device, utils::load_file("shader/rt_basic.rgen.spv"));
        CVK_ASSERT(rgen.create() == VK_SUCCESS);
        cvk::Shader rmiss(_device, utils::load_file("shader/rt_basic.rmiss.spv"));
        CVK_ASSERT(rmiss.create() == VK_SUCCESS);
        cvk::Shader rchit(_device, utils::load_file("shader/rt_basic.rchit.spv"));
        CVK_ASSERT(rchit.create() == VK_SUCCESS);

        layout.attaches(descriptor[0].get_layout());
        CVK_ASSERT(layout.create() == VK_SUCCESS);

        pipeline.add_stage(VK_SHADER_STAGE_RAYGEN_BIT_KHR, rgen);
        pipeline.add_stage(VK_SHADER_STAGE_MISS_BIT_KHR, rmiss);
        pipeline.add_stage(VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR, rchit);
        CVK_ASSERT(pipeline.create(layout) == VK_SUCCESS);
    }

    void setup_sbt()
    {
        std::cout << "setup sbt\n";
        auto properties = _device.get_raytracing_properties();
        auto aligned_size = cvk_aligned_size(properties.shaderGroupHandleSize, properties.shaderGroupHandleAlignment);
        auto stages_size = 3;
        auto seri_size = aligned_size * stages_size;

        std::vector<char> serial(seri_size);
        CVK_IMPORT_FUNCTION(_device, vkGetRayTracingShaderGroupHandlesKHR);
        vkGetRayTracingShaderGroupHandlesKHR(_device, pipeline, 0, stages_size, seri_size, serial.data());

        rgen_sbt_buffer.upload(&serial[0 * aligned_size], aligned_size);
        rmiss_sbt_buffer.upload(&serial[1 * aligned_size], aligned_size);
        rchit_sbt_buffer.upload(&serial[2 * aligned_size], aligned_size);
    }

    void build_cmdbuffer(uint32_t width, uint32_t height)
    {
        std::cout << "build command buffer\n";

        auto properties = _device.get_raytracing_properties();
        auto aligned_size = cvk_aligned_size(properties.shaderGroupHandleSize, properties.shaderGroupHandleAlignment);

        VkStridedDeviceAddressRegionKHR raygenShaderSbtEntry{};
        raygenShaderSbtEntry.deviceAddress = rgen_sbt_buffer.get_address();
        raygenShaderSbtEntry.stride = aligned_size;
        raygenShaderSbtEntry.size = aligned_size;

        VkStridedDeviceAddressRegionKHR missShaderSbtEntry{};
        missShaderSbtEntry.deviceAddress = rmiss_sbt_buffer.get_address();
        missShaderSbtEntry.stride = aligned_size;
        missShaderSbtEntry.size = aligned_size;

        VkStridedDeviceAddressRegionKHR hitShaderSbtEntry{};
        hitShaderSbtEntry.deviceAddress = rchit_sbt_buffer.get_address();
        hitShaderSbtEntry.stride = aligned_size;
        hitShaderSbtEntry.size = aligned_size;

        VkStridedDeviceAddressRegionKHR callableShaderSbtEntry{};

        cmdbuf.create();
        cmdbuf.begin();
        cmdbuf.cmd().bind_descriptor_sets(VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, layout, { descriptor[0] });
        cmdbuf.cmd().bind_pipeline(VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, pipeline);
        
        CVK_IMPORT_FUNCTION(_device, vkCmdTraceRaysKHR);
        vkCmdTraceRaysKHR(
            cmdbuf,
            &raygenShaderSbtEntry,
            &missShaderSbtEntry,
            &hitShaderSbtEntry,
            &callableShaderSbtEntry,
            width,
            height,
            1);

        cmdbuf.end();
    }

    void draw(VkFence fence)
    {
        _queue.submit({ cmdbuf }, fence);
    }

    auto& get_storage_image() const { return storage_image; }

};


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

    cvk::Instance instance(instance_extensions, instance_layers);
    std::vector<VkPhysicalDevice>&& devices = instance.get_all_physical_device();
    VkPhysicalDeviceFeatures device_features = {};
    cvk::Device device(devices[0]);
    device.add_extensions(
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
        VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
        VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
        VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    );
    device.add_device_features(
        cvk::DeviceFeatureAcceleration(),
        cvk::DeviceFeatureBufferAddress(),
        cvk::DeviceFeatureRayTracing()
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

    std::vector<VkSurfaceFormatKHR> formats;
    __cvk::get_surface_formats(device.get_physical_device(), surface, formats);
    CVK_ASSERT(formats.size() > 0);
    auto format = formats[0].format;
    cvk::Swapchain swapchain(device, device.get_physical_device(), surface, { VK_PRESENT_MODE_FIFO_KHR }, formats[0]);
    swapchain.set_image_usage(VK_IMAGE_USAGE_TRANSFER_DST_BIT);
    swapchain.create();

    uint32_t graphics_index = UINT32_MAX, present_index = UINT32_MAX;
    __cvk::get_queue_family_index(device.get_physical_device(), VK_QUEUE_GRAPHICS_BIT, graphics_index);
    __cvk::get_queue_family_index_present_support(device.get_physical_device(), surface, VK_QUEUE_GRAPHICS_BIT, present_index);
    CVK_ASSERT(graphics_index != UINT32_MAX);
    CVK_ASSERT(present_index != UINT32_MAX);


    cvk::Queue queue(device, graphics_index);
    cvk::CommandPool cmdpool(device, graphics_index);
    cmdpool.create();
    std::vector<cvk::CommandBuffer> presentbufs;
    cvk::Fence fence(device);
    fence.create();
    cvk::Semaphore pressemaphore(device);
    pressemaphore.create();

    AccStructTriangle triangle(device, cmdpool, queue, swapchain.get_images().size());
    triangle.create_bottom_level();
    triangle.create_top_level();
    triangle.create_raytracing_image(width, height);
    triangle.create_sbt();
    triangle.create_descriptor(width, height);
    triangle.create_raytracing_pipeline();
    triangle.setup_sbt();
    triangle.build_cmdbuffer(width, height);

    auto presents = swapchain.get_images();
    std::vector<cvk::ColorImageView2D> presentimage;
    for (auto& p : presents) {
        auto& pi = presentimage.emplace_back(device);
        pi.create(format, p);

        auto& presentbuf = presentbufs.emplace_back(device, cmdpool);
        presentbuf.create();

        presentbuf.begin();

        presentbuf.cmd().set_image_layout_barrier(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, triangle.get_storage_image(), triangle.get_storage_image().get_subresource_range());
        presentbuf.cmd().set_image_layout_barrier(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, p, pi.get_subresource_range());
        presentbuf.cmd().copy_image(triangle.get_storage_image(), p, VK_IMAGE_ASPECT_COLOR_BIT, { width, height, 1 });
        presentbuf.cmd().set_image_layout_barrier(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL, triangle.get_storage_image(), triangle.get_storage_image().get_subresource_range());
        presentbuf.cmd().set_image_layout_barrier(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, p, pi.get_subresource_range());

        presentbuf.end();
    }

    auto index = swapchain.acquire(pressemaphore);
    triangle.draw(fence);
    fence.wait();
    fence.reset();
    queue.submit({ presentbufs[index] }, fence);
    fence.wait();
    fence.reset();

    swapchain.present(queue, {});

    getchar();

}