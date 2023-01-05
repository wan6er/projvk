#pragma once

#include "cvk/image/base_image.h"

namespace cvk
{
    class CVK_API Image2D : public BaseImage
    {
    public:
        explicit Image2D(VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags usage, VkImageTiling tiling);  
        explicit Image2D(VkImage image);
        Image2D(Image2D CONST_REFERENCE image) = default;
        // Image2D(VkDevice device);
        ~Image2D();

        // void setup(VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags usage);
    };
};