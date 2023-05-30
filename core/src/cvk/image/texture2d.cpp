#include "cvk/image/texture2d.h"
#include "cvk/initialize/image_initialize.h"

namespace cvk
{


VkResult BaseTexture2D::create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property, 
    VkFormat format, uint32_t width, uint32_t height, VkImageLayout image_layout, VkImageUsageFlags usage, 
    VkImageTiling tiling, VkImageAspectFlags aspect)
{
    VkImageType image_type = VK_IMAGE_TYPE_2D;
    basic::Texture::setup(format, { width, height, 1 }, image_type, image_layout, usage, tiling, aspect);

    return basic::Texture::create(properties, property);
}


};