#include "cvk/image/texture.h"

namespace cvk::basic
{
    
Texture::Texture(VkDevice device) :
    _MemImage(device), _ImageView(device)
{
}

void Texture::setup(VkFormat format, VkExtent3D extent, VkImageType type, VkImageLayout layout, VkImageUsageFlags usage, VkImageTiling tiling, VkImageAspectFlags aspect)
{
    _MemImage::setup(format, extent, type, layout, usage, tiling);
    _ImageView::setup(format, static_cast<VkImageViewType>(type), aspect);
}

VkResult Texture::create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property)
{
    VkResult result = VK_SUCCESS;
    if ((result = _MemImage::create_obj())) {
        return result;
    }

    if ((result = _MemImage::create_mem(properties, property))) {
        return result;
    }

    if ((result = _MemImage::bind())) {
        return result;
    }
    return _ImageView::create(*this);
}

void Texture::release()
{
    _ImageView::release();
    _MemImage::release();
    _MemImage::deallocate();
}


} // namespace cvk
