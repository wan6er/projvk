#ifdef CVK_RAYTRACING

#include "test_header.h"

#include <vector>
#include <string>

#include "cvk/instance.h"
#include "cvk/device.h"

#include "cvk/buffer.h"
#include "cvk/acceleration_structure.h"
#include "cvk/command_pool.h"
#include "cvk/command_buffer.h"
#include "cvk/fence.h"

class AccStructTest
{
public:
    AccStructTest(cvk::Device& device, cvk::CommandPool& cmdpool, cvk::Queue& queue) :
        _device(device), _cmdpool(cmdpool), _queue(queue),
        vertex_buffer(device), index_buffer(device), bottom_transform_buffer(device),
        bottom_buffer(device), bottom(device),
        top_transform_buffer(device), top_buffer(device), top(device)
    {
    }

    void create_bottom_level()
    {
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

        CHECK(vertex_buffer.create(_device.get_memory_properties(), sizeof(Vertex) * vertices.size()) == VK_SUCCESS);
        vertex_buffer.upload(vertices.data(), sizeof(Vertex) * vertices.size());
        CHECK(index_buffer.create(_device.get_memory_properties(), sizeof(uint32_t) * indices.size()) == VK_SUCCESS);
        index_buffer.upload(indices.data(), sizeof(uint32_t) * indices.size());
        CHECK(bottom_transform_buffer.create(_device.get_memory_properties(), sizeof(VkTransformMatrixKHR)) == VK_SUCCESS);
        bottom_transform_buffer.upload(&transformMatrix, sizeof(VkTransformMatrixKHR));

        cvk::AccStructGeometry geometry;
        auto& trigeometry = geometry.triangles();
        trigeometry.set_vertex_device_address(vertex_buffer.get_address());
        trigeometry.set_vertex_format(VK_FORMAT_R32G32B32_SFLOAT);
        trigeometry.set_vertex_max_num(3);
        trigeometry.set_index_device_address(index_buffer.get_address());
        trigeometry.set_index_type(VK_INDEX_TYPE_UINT32);
        trigeometry.set_transform_device_address(bottom_transform_buffer.get_address());
        
        cvk::AccStructGeometries geometries;
        geometries.set_geometries_type(VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR);
        geometries.attach(geometry);

        auto geosize = geometries.get_geometries_size(_device, VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR);

        cvk::AccelerationStructureScratchBuffer scratch_bottom(_device);
        CHECK(scratch_bottom.create(_device.get_memory_properties(), geosize.buildScratchSize) == VK_SUCCESS);

        CHECK(bottom_buffer.create(_device.get_memory_properties(), geosize.accelerationStructureSize) == VK_SUCCESS);

        CHECK(bottom.create(VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR, bottom_buffer, geosize.accelerationStructureSize) == VK_SUCCESS);

        cvk::CommandBuffer cmdbuffer(_device, _cmdpool);
        CHECK(cmdbuffer.create() == VK_SUCCESS);

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
        CHECK(_queue.submit({ cmdbuffer }, fence) == VK_SUCCESS);

        fence.wait();

    }

    void create_top_level()
    {
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

        top_transform_buffer.create(_device.get_memory_properties(), sizeof(VkTransformMatrixKHR));
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
        CHECK(_queue.submit({ cmdbuffer }, fence) == VK_SUCCESS);

        fence.wait();
    }

private:
    cvk::Device& _device;
    cvk::CommandPool& _cmdpool;
    cvk::Queue& _queue;

    cvk::AccelerationStructureInputBufferWritable vertex_buffer;
    cvk::AccelerationStructureInputBufferWritable index_buffer;
    cvk::AccelerationStructureInputBufferWritable bottom_transform_buffer;
    cvk::AccelerationStructureBuffer bottom_buffer;
    cvk::AccelerationStructure bottom;

    cvk::AccelerationStructureInputBufferWritable top_transform_buffer;
    cvk::AccelerationStructureBuffer top_buffer;
    cvk::AccelerationStructure top;

};

TEST_FUNC_BEGIN("acceleration struct")
    std::vector<std::string> instance_extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#ifdef WIN32
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
    };
    std::vector<std::string> instance_layers = {
        "VK_LAYER_KHRONOS_validation"
    };

    cvk::Instance instance(instance_extensions, instance_layers);
    std::vector<VkPhysicalDevice>&& devices = instance.get_all_physical_device();
    VkPhysicalDeviceFeatures device_features { };
    cvk::Device device(devices[0]);
    device.add_extensions(
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
        VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
        VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
    );
    device.add_device_features(
        cvk::DeviceFeatureAcceleration(),
        cvk::DeviceFeatureBufferAddress(),
        cvk::DeviceFeatureRayTracing()
    );
    CHECK(device.create(VK_QUEUE_GRAPHICS_BIT) == VK_SUCCESS);

    uint32_t grapindex;
    __cvk::get_queue_family_index(device.get_physical_device(), VK_QUEUE_GRAPHICS_BIT, grapindex);
    cvk::CommandPool cmdpool(device, grapindex);
    CHECK(cmdpool.create() == VK_SUCCESS);
    cvk::Queue queue(device, grapindex);


    AccStructTest test(device, cmdpool, queue);
    test.create_bottom_level();
    test.create_top_level();

TEST_FUNC_END

#endif