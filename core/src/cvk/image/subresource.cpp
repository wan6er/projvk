#include "cvk/image/subresource.h"

namespace cvk
{

Subresource::Subresource() noexcept :
    aspect(VK_IMAGE_ASPECT_NONE),
    mip_levels(1),
    array_layers(1)
{
}

void Subresource::set_subresource_aspect(VkImageAspectFlags aspect)
{
    this->aspect = aspect;
}

void Subresource::set_subresource_miplevels(uint32_t mip_levels)
{
    this->mip_levels = mip_levels;
}

void Subresource::set_subresource_arraylayers(uint32_t array_layers)
{
    this->array_layers = array_layers;
}
    
VkImageSubresourceRange Subresource::get_all_subresource() const
{
    VkImageSubresourceRange range = {};
    range.aspectMask = aspect;
    range.levelCount = mip_levels;
    range.layerCount = array_layers;
    return range;
}

VkImageSubresourceLayers Subresource::get_subresource_layers(uint32_t mip_level) const
{
    VkImageSubresourceLayers layers = {};
    layers.aspectMask = aspect;
    layers.mipLevel = mip_level;
    layers.layerCount = array_layers;
    return layers;
}

VkImageSubresource Subresource::get_subresource(uint32_t mip_level, uint32_t layer) const
{
    VkImageSubresource sub = {};
    sub.aspectMask = aspect;
    sub.mipLevel = mip_level;
    sub.arrayLayer = layer;
    return sub;
}

} // namespace cvk
