#include "cvk/image/image2d.h"

cvk::Image2D::Image2D(VkDevice device, VkFormat format, const VkExtent2D &extent, VkImageUsageFlags usage) :
    Image(device, ImageType, format, ImageLayers, extent, usage)
{

}

cvk::Image2D::~Image2D()
{

}