#pragma once

#include "cvk/vk_header.h"

namespace cvk
{
    class CVK_API SubpassDependency : public VkSubpassDependency
    {
    public:
        SubpassDependency(uint32_t src_subpass, uint32_t dst_subpass);

        auto set_src(VkPipelineStageFlags stage_mask, VkAccessFlags access_mask) -> SubpassDependency&;
        auto set_dst(VkPipelineStageFlags stage_mask, VkAccessFlags access_mask) -> SubpassDependency&;
        
    };
};