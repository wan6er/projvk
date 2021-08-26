#pragma once

#include "cvk/image/image.h"

namespace cvk
{
    class Image2D : public Image
    {
    public:
        Image2D(VkDevice device, VkFormat format, const VkExtent2D& extent, VkImageUsageFlags usage);
        ~Image2D();

    private:
        constexpr static VkImageType ImageType = VK_IMAGE_TYPE_2D;
        constexpr static uint32_t ImageLayers = 1;
    };
};