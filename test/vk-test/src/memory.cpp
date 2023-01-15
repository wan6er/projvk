#include "test_header.h"

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/buffer.h"
#include "cvk/memory.h"
#include "cvk/memorized_buffer.h"
#include "cvk/initialize/memory_initialize.h"

TEST_FUNC_BEGIN("memory")

    std::vector<std::string> instance_extensions = {
        // VK_KHR_SURFACE_EXTENSION_NAME,
        // VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        // VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };
    std::vector<std::string> instance_layers = {
        // "VK_LAYER_KHRONOS_validation"
    };
    std::vector<std::string> device_extensions = {
        // VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        // VK_EXT_DEBUG_MARKER_EXTENSION_NAME
    };

    cvk::Instance instance(instance_extensions, instance_layers);
    std::vector<VkPhysicalDevice>&& devices = instance.get_all_physical_device();
    VkPhysicalDeviceFeatures device_features = {};
    cvk::Device device(devices[0], device_extensions, device_features, VK_QUEUE_GRAPHICS_BIT);
    
    // vkCmdCopyBuffer()
    // uint32_t index;
    // __cvk::get_memory_type(device.get_memory_properties(), , VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, index);
    // cvk::Memory memory(device, 10, 0);
    {
        VkBuffer buffer;
        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = 65536;
        bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

        auto result = vkCreateBuffer(device, &bufferCreateInfo, nullptr, &buffer);
        CHECK(result == VK_SUCCESS);

        VkMemoryRequirements requirement;
        vkGetBufferMemoryRequirements(device, buffer, &requirement);
    }

    {
        VkBuffer buffer;
        VkBufferCreateInfo info = {};
        __cvk::get_default_buffer_create_info(65535, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, {}, info);
        auto result = __cvk::create_buffer(device, info, buffer);
        CHECK(result == VK_SUCCESS);

        VkMemoryRequirements requirement;
        __cvk::get_memory_requirement(device, buffer, requirement);

        VkDeviceMemory memory;
        result = __cvk::alloc_memory(device, device.get_memory_properties(), requirement, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, memory);
        CHECK(result == VK_SUCCESS);

        result = __cvk::bind_memory(device, buffer, memory);
        CHECK(result == VK_SUCCESS);

        int* data = nullptr;
        result = __cvk::map_memory(device, memory, 4, data);
        CHECK(result == VK_SUCCESS);

        int a = 10;
        memcpy(data, &a, sizeof(a));

        __cvk::unmap_memory(device, memory);

        __cvk::destroy_buffer(device, buffer);
        __cvk::free_memory(device, memory);
    }

    {
        cvk::Buffer buffer(device);
        auto result = buffer.create(65535, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        CHECK(result == VK_SUCCESS);

        cvk::Memory memory(device);
        result = memory.allocate(device.get_memory_properties(), buffer.get_memory_requirement(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        CHECK(result == VK_SUCCESS);
        result = memory.bind(buffer);
        CHECK(result == VK_SUCCESS);
    }

    {
        cvk::Buffer buffer(device);
        auto result = buffer.create(65535, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        CHECK(result == VK_SUCCESS);

        cvk::Buffer buffer1(device);
        result = buffer1.create(65535, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        CHECK(result == VK_SUCCESS);
        
        cvk::Memory memory(device);
        result = memory.allocate(device.get_memory_properties(), buffer1.get_memory_requirement(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        CHECK(result == VK_SUCCESS);
        result = memory.bind(buffer);
        CHECK(result == VK_SUCCESS);
    }
    {
        cvk::MemorizedBuffer mem_buffer(device);
        CHECK(mem_buffer.create(device.get_memory_properties(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 512, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) == VK_SUCCESS);
    }

TEST_FUNC_END