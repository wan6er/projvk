#pragma once

#include "cvk/vk_header.h"
#include "cvk/base/base_memorized.h"

#include "base_image.h"
#include "base_image_view.h"

namespace cvk::basic
{
    
    class CVK_API Texture : 
        public BaseMemorized<basic::Image>,
        public basic::ImageView
    {
    public:
        using _MemImageType = BaseMemorized<basic::Image>;
        using _ImageViewType = basic::ImageView;

        Texture(VkDevice device);
        virtual ~Texture() = default;

        void setup(VkFormat format, VkExtent3D extent, VkImageType type, VkImageLayout layout, VkImageUsageFlags usage, VkImageTiling tiling, VkImageAspectFlags aspect);

        VkResult create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property);

    protected:
        void release();

    };

} // namespace cvk
