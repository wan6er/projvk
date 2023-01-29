#pragma once

#include "cvk/vk_header.h"

namespace cvk
{

    struct CVK_API Subresource
    {
        VkImageAspectFlags aspect;
        uint32_t mip_levels;
        uint32_t array_layers;

        Subresource() noexcept;

        void set_subresource_aspect(VkImageAspectFlags aspect);
        void set_subresource_miplevels(uint32_t mip_levels);
        void set_subresource_arraylayers(uint32_t array_layers);

        VkImageSubresourceRange get_all_subresource() const;
        VkImageSubresourceLayers get_subresource_layers(uint32_t mip_level) const;
        VkImageSubresource get_subresource(uint32_t mip_level, uint32_t layer) const;

    };

} // namespace cvk
