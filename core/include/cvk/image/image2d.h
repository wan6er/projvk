#pragma once

#include "cvk/image/base_image.h"

namespace cvk
{
    class CVK_API Image2D : public BaseImage
    {
    public:
        Image2D(VkDevice device);
        Image2D(VkDevice device, VkImage image);
        Image2D(Image2D CONST_REFERENCE image) = default;
        // Image2D(VkDevice device);
        ~Image2D();

        VkResult create(VkFormat format, uint32_t width, uint32_t height, VkImageLayout image_layout, VkImageUsageFlags usage, VkImageTiling tiling);
        // void setup(VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags usage);
    };
};