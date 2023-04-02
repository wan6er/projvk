#pragma once

#include "ktx.h"

#include "cvk/device.h"
#include "cvk/image.h"
#include "cvk/buffer.h"
#include "cvk/fence.h"
#include "cvk/command_pool.h"
#include "cvk/command_buffer.h"

void load_texture(cvk::Device CONST_REFERENCE device, std::string filename, uint32_t graphics_index, cvk::StandardTexture2D& texture)
{
    // "texture/colored_glass_rgba.ktx"
    ktxTexture* target = nullptr;
    ktxResult result = ktxTexture_CreateFromNamedFile(filename.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &target);
    CVK_ASSERT(result == KTX_SUCCESS);

    cvk::TransferSrcBuffer texture_stage(device);
    CVK_ASSERT(texture_stage.create(device.get_memory_properties(), target->dataSize) == VK_SUCCESS);
    CVK_ASSERT(texture_stage.upload(target->pData, texture_stage.get_size()) == VK_SUCCESS);

    // cvk::StandardTexture2D texture(device);
    texture.set_subresource_miplevels(target->numLevels);
    CVK_ASSERT(texture.create(device.get_memory_properties(), VK_FORMAT_R8G8B8A8_UNORM, target->baseWidth, target->baseHeight) == VK_SUCCESS);

    cvk::CommandPool transfer_pool(device, graphics_index);
    CVK_ASSERT(transfer_pool.create() == VK_SUCCESS);
    cvk::CommandBuffer transfer_buf(device, transfer_pool);
    CVK_ASSERT(transfer_buf.create() == VK_SUCCESS);
    cvk::Queue queue(device, graphics_index);
    cvk::Fence fence(device);
    CVK_ASSERT(fence.create() == VK_SUCCESS);

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

    transfer_buf.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    transfer_buf.cmd().set_barrier(VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT)
        .add_image_barrier(texture, texture.get_all_subresource(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        .apply();
    transfer_buf.cmd().copy_buffer_image(texture_stage, texture, image_copies);
    transfer_buf.cmd().set_barrier(VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT)
        .add_image_barrier(texture, texture.get_all_subresource(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        .apply();
    transfer_buf.end();

    CVK_ASSERT(queue.submit({ transfer_buf }, fence) == VK_SUCCESS);
    CVK_ASSERT(fence.wait() == VK_SUCCESS);

    ktxTexture_Destroy(target);
}
