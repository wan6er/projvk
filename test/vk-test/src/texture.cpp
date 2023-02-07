#include "test_header.h"

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/command_pool.h"
#include "cvk/command_buffer.h"
#include "cvk/command/queue.h"
#include "cvk/fence.h"
#include "cvk/image.h"
#include "cvk/memorized_buffer.h"

#include "ktx.h"

#include <string>


TEST_FUNC_BEGIN("texture")

    std::vector<std::string> instance_extensions = {
    };
    std::vector<std::string> instance_layers = {
    };
    std::vector<std::string> device_extensions = {
    };

    cvk::Instance instance(instance_extensions, instance_layers);
    std::vector<VkPhysicalDevice>&& devices = instance.get_all_physical_device();
    VkPhysicalDeviceFeatures device_features = {};
    cvk::Device device(devices[0], device_extensions, device_features, VK_QUEUE_GRAPHICS_BIT);


    std::string filename = "texture/colored_glass_rgba.ktx";
    ktxTexture* target = nullptr;
    ktxResult result = ktxTexture_CreateFromNamedFile(filename.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &target);
    CHECK(result == KTX_SUCCESS);

    {
        cvk::WritableTexture2D texture(device);
        CHECK(texture.create(
            device.get_memory_properties(), VK_FORMAT_R8G8B8A8_UNORM, target->baseWidth, target->baseHeight) == VK_SUCCESS);

        void* ptr = nullptr;
        CHECK(texture.map(ptr) == VK_SUCCESS);
        memcpy(ptr, ktxTexture_GetData(target), texture.get_size());
        texture.unmap();
        
        
        CHECK(cvk::StandardTexture2D(device).create(
            device.get_memory_properties(), VK_FORMAT_R8G8B8A8_UNORM, target->baseWidth, target->baseHeight) == VK_SUCCESS);
        CHECK(cvk::WritableTexture2DFullSpeed(device).create(
            device.get_memory_properties(), VK_FORMAT_R8G8B8A8_UNORM, target->baseWidth, target->baseHeight) == VK_SUCCESS);
            
        CHECK(cvk::StandardColorAttachment2D(device).create(
            device.get_memory_properties(), VK_FORMAT_R8G8B8A8_UNORM, target->baseWidth, target->baseHeight) == VK_SUCCESS);
        CHECK(cvk::ReadableColorAttachment2D(device).create(
            device.get_memory_properties(), VK_FORMAT_R8G8B8A8_UNORM, target->baseWidth, target->baseHeight) == VK_SUCCESS);
    }

    {
        uint32_t transfer_queue_index = device.get_queue_family_index(VK_QUEUE_GRAPHICS_BIT);
        cvk::CommandPool cmd_pool(device, transfer_queue_index);
        cmd_pool.transient();
        CHECK(cmd_pool.create() == VK_SUCCESS);
        cvk::CommandBuffer cmd(device, cmd_pool);
        CHECK(cmd.create() == VK_SUCCESS);
        cvk::Queue queue(device, transfer_queue_index);
        cvk::Fence fence(device);
        CHECK(fence.create() == VK_SUCCESS);

        cvk::TransferSrcBuffer src(device);
        CHECK(src.create(device.get_memory_properties(), ktxTexture_GetSize(target)) == VK_SUCCESS);

        cvk::StandardTexture2D texture(device);
        texture.set_subresource_miplevels(target->numLevels);
        CHECK(texture.create(
            device.get_memory_properties(), VK_FORMAT_R8G8B8A8_UNORM, target->baseWidth, target->baseHeight) == VK_SUCCESS);

        std::vector<VkBufferImageCopy> image_copies(target->numLevels);
        for (uint32_t i = 0; i < target->numLevels; ++i) {
            ktx_size_t offset;
            KTX_error_code ret = ktxTexture_GetImageOffset(target, i, 0, 0, &offset);
            image_copies[i].bufferOffset = offset;
            image_copies[i].imageSubresource = texture.get_subresource_layers(i);
            image_copies[i].imageExtent.width = target->baseWidth >> i;
            image_copies[i].imageExtent.height = target->baseHeight >> i;
            image_copies[i].imageExtent.depth = 1;
        }

        cmd.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        cmd.cmd().set_barrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT)
            .add_image_barrier(texture, texture.get_all_subresource(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
            .apply();
        cmd.cmd().copy_buffer_image(src, texture, image_copies);
        cmd.cmd().set_barrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT)
            .add_image_barrier(texture, texture.get_all_subresource(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
            .apply();
        cmd.end();

        CHECK(queue.submit({ cmd }, fence) == VK_SUCCESS);
        CHECK(fence.wait() == VK_SUCCESS);
    }

    ktxTexture_Destroy(target);

TEST_FUNC_END