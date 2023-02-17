#pragma once

#include "cvk/vk_header.h"

namespace __cvk
{
    // image
    CVK_API void get_default_image_create_info(VkImageCreateInfo& create_info);
    CVK_API void get_image_create_info(VkFormat format, VkExtent3D CONST_REFERENCE extent, VkImageType type, VkImageLayout image_layout, VkImageUsageFlags usage, VkImageTiling tiling, VkImageCreateInfo& create_info);
    CVK_API void get_default_image_view_create_info(VkImageViewCreateInfo& create_info);
    CVK_API void get_image_view_create_info(VkFormat format, VkImageViewType type, VkImageAspectFlags aspect, VkImage image, VkImageViewCreateInfo& create_info);
    CVK_API void get_image_view_create_info(VkFormat format, VkImageViewType type, VkImage image, VkImageSubresourceRange CONST_REFERENCE subresource, VkImageViewCreateInfo& create_info);
    CVK_API VkResult create_image(VkDevice device, const VkImageCreateInfo& image_create_info, VkImage& image);
    CVK_API VkResult create_image(VkDevice device, VkImageType type, VkFormat format, uint32_t layers, const VkExtent2D& extent, VkImageUsageFlags usage, VkImage& image);
    CVK_API VkResult create_image2d(VkDevice device, VkFormat format, const VkExtent2D& extent, VkImageUsageFlags usage, VkImage& image);
    CVK_API void destroy_image(VkDevice device, VkImage image);

    // image view
    CVK_API VkResult create_image_view(VkDevice device, const VkImageViewCreateInfo& create_info, VkImageView& image_view);
    CVK_API VkResult create_image_view(VkDevice device, VkImageViewType type, VkImage image, VkFormat format, uint32_t layers, VkImageAspectFlags image_aspect, VkImageView& image_view);
    CVK_API VkResult create_image_view2d(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags image_aspect, VkImageView& image_view);
    CVK_API void destroy_image_view(VkDevice device, VkImageView image_view);

    // sampler
    CVK_API void get_default_sampler_create_info(VkSamplerCreateInfo& create_info);
    CVK_API VkResult create_sampler(VkDevice device, VkSamplerCreateInfo CONST_REFERENCE create_info, VkSampler& sampler);
    CVK_API void destroy_sampler(VkDevice device, VkSampler sampler);

};