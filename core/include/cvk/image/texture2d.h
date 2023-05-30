#pragma once

#include "cvk/base/base_memorized.h"
#include "cvk/image/texture.h"

namespace cvk
{
    
    class CVK_API BaseTexture2D : public basic::Texture
    {
    public:
        using basic::Texture::Texture;

        VkResult create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property, 
            VkFormat format, uint32_t width, uint32_t height, VkImageLayout image_layout, VkImageUsageFlags usage, VkImageTiling tiling, VkImageAspectFlags aspect);
    };

    template<VkMemoryPropertyFlags _Property, VkImageLayout _Layout, VkImageUsageFlags _Usage, VkImageTiling _Tiling, VkImageAspectFlags _Aspect>
    class BaseTypeTexture2D : public BaseTexture2D
    {
    public:
        using BaseTexture2D::BaseTexture2D;

        inline VkResult create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkFormat format, uint32_t width, uint32_t height)
        {
            return BaseTexture2D::create(properties, _Property, format, width, height, _Layout, _Usage, _Tiling, _Aspect);
        }
    };

};