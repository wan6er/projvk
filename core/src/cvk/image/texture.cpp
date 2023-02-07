#include "cvk/image/texture.h"

namespace cvk::basic
{
    
Texture::Texture(VkDevice device) :
    _MemImageType(device), _ImageViewType(device)
{
}

void Texture::setup(VkFormat format, VkExtent3D extent, VkImageType type, VkImageLayout layout, VkImageUsageFlags usage, VkImageTiling tiling, VkImageAspectFlags aspect)
{
    _MemImageType::setup(format, extent, type, layout, usage, tiling);
    _ImageViewType::setup(format, static_cast<VkImageViewType>(type), aspect);
}

VkResult Texture::create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property)
{
    VkResult result = VK_SUCCESS;
    result = _MemImageType::create(properties, property);
    if (result != VK_SUCCESS) {
        return result;
    }
    return _ImageViewType::create(*this);
}

void Texture::release()
{
    _ImageViewType::release();
    _MemImageType::release();
    _MemImageType::deallocate();
}


} // namespace cvk
