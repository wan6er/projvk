#include "cvk/image/image2d.h"
#include "cvk/initialize/image_initialize.h"

namespace cvk
{

Image2D::Image2D(VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags usage, VkImageTiling tiling)
{
    __cvk::get_default_image_create_info(format, { width, height, 1 }, usage, tiling, image_info());
    // Image2D::setup(format, width, height, usage);
}

Image2D::Image2D(VkImage image) :
    BaseImage(image)
{
}

Image2D::~Image2D()
{

}

// void Image2D::setup(VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags usage)
// {
// }

};