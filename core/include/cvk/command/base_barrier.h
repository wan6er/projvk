#pragma once

#include "cvk/vk_header.h"
#include "utils/base_multi_attach.h"

namespace cvk
{
    
    struct CVK_API BaseBarrier :
        public utils::BaseMultipleAttaches<VkBufferMemoryBarrier, VkImageMemoryBarrier>
    {
        VkCommandBuffer buffer;
        VkPipelineStageFlags src;
        VkPipelineStageFlags dst;
        
        BaseBarrier(VkCommandBuffer buffer);
        
        BaseBarrier& add_image_barrier(VkImage image, VkImageSubresourceRange CONST_REFERENCE subresource, VkImageLayout src, VkImageLayout dst);
        void apply();

    };

}