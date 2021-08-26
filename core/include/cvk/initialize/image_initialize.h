#pragma once

#include "cvk/vk_header.h"

namespace __cvk
{
    VkResult create_image(VkDevice device, VkImageType type, VkFormat format, uint32_t layers, const VkExtent2D& extent, VkImageUsageFlags usage, VkImage& image);
    VkResult create_image2d(VkDevice device, VkFormat format, const VkExtent2D& extent, VkImageUsageFlags usage, VkImage& image);
    VkResult create_image_view2d(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags image_aspect, VkImageView& image_view);
    VkResult bind_memory_to_image(VkDevice device, VkImage image, VkDeviceMemory memory);
    void destroy_image(VkDevice device, VkImage image);

    void get_image_memory_requirement(VkDevice device, VkImage texture, VkMemoryRequirements& requirement);
};