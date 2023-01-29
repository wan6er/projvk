#include "cvk/command/base_barrier.h"
#include "cvk/initialize/command_initialize.h"

namespace cvk
{

BaseBarrier::BaseBarrier(VkCommandBuffer buffer) :
    buffer(buffer)
{
}
    
BaseBarrier& BaseBarrier::add_image_barrier(VkImage image, VkImageSubresourceRange CONST_REFERENCE subresource, VkImageLayout src, VkImageLayout dst)
{
    VkImageMemoryBarrier image_barrier = {};
    __cvk::get_image_memory_barrier(image, subresource, src, dst, image_barrier);
    attaches(image_barrier);
    return *this;
}

void BaseBarrier::apply()
{
    std::vector<VkBufferMemoryBarrier> CONST_REFERENCE buffer_barrier = *this;
    std::vector<VkImageMemoryBarrier> CONST_REFERENCE image_barrier = *this;
    __cvk::cmd_set_pipeline_barrier(buffer, src, dst, buffer_barrier, image_barrier);
}

} // namespace cvk
