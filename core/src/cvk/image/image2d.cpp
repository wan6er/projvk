#include "cvk/image/image2d.h"
#include "cvk/initialize/image_initialize.h"

namespace cvk
{

Image2D::Image2D(VkDevice device) :
    BaseImage(device)
{
    // Image2D::setup(format, width, height, usage);
}

Image2D::Image2D(VkDevice device, VkImage image) :
    BaseImage(device, image)
{
}

Image2D::~Image2D()
{

}

VkResult Image2D::create(VkFormat format, uint32_t width, uint32_t height, VkImageLayout image_layout, VkImageUsageFlags usage, VkImageTiling tiling)
{
    __cvk::get_default_image_create_info(format, { width, height, 1 }, image_layout, usage, tiling, get_image_info());
    return __cvk::create_image(get_device(), get_image_info(), object());
}

// void Image2D::setup(VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags usage)
// {
// }

};