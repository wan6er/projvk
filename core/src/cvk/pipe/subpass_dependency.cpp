#include "cvk/pipe/subpass_dependency.h"

namespace cvk
{

SubpassDependency::SubpassDependency(uint32_t src_subpass, uint32_t dst_subpass) :
    VkSubpassDependency()
{
    srcSubpass = src_subpass;
    dstSubpass = dst_subpass;
}

auto SubpassDependency::set_src(VkPipelineStageFlags stage_mask, VkAccessFlags access_mask) -> SubpassDependency&
{
    srcStageMask = stage_mask;
    srcAccessMask = access_mask;
    return *this;
}

auto SubpassDependency::set_dst(VkPipelineStageFlags stage_mask, VkAccessFlags access_mask) -> SubpassDependency&
{
    dstStageMask = stage_mask;
    dstAccessMask = access_mask;
    return *this;
}

};