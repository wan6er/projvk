#include "cvk/initialize/image_initialize.h"



namespace __cvk
{

VkResult create_image(VkDevice device, const VkImageCreateInfo& image_create_info, VkImage& image)
{
	CVK_ASSERT(device != VK_NULL_HANDLE);
	return vkCreateImage(device, &image_create_info, nullptr, &image);
}

VkResult create_image(VkDevice device, VkImageType type, VkFormat format, uint32_t layers, const VkExtent2D &extent, VkImageUsageFlags usage, VkImage &image)
{
	CVK_ASSERT(device != VK_NULL_HANDLE);
	VkImageCreateInfo image_create_info{};
	image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_create_info.imageType = type;
	image_create_info.format = format;
	image_create_info.extent = { extent.width, extent.height, 1 };
	image_create_info.mipLevels = 1;
	image_create_info.arrayLayers = layers;
	image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
	image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
	image_create_info.usage = usage;

	return create_image(device, image_create_info, image);
}

void get_default_image_create_info(VkFormat format, const VkExtent3D& extent, VkImageLayout image_layout, VkImageUsageFlags usage, VkImageTiling tiling, VkImageCreateInfo& create_info)
{
    create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    create_info.imageType = VK_IMAGE_TYPE_2D;
    create_info.format = format;
    create_info.extent = extent;
    create_info.mipLevels = 1;
    create_info.arrayLayers = 1;
    create_info.samples = VK_SAMPLE_COUNT_1_BIT;
    create_info.tiling = tiling;
    create_info.usage = usage;
    create_info.initialLayout = image_layout;
}

void get_default_image_view_create_info(VkFormat format, VkImageViewType type, VkImageAspectFlags aspect, VkImage image, VkImageViewCreateInfo& create_info)
{
    create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.image = image;
    create_info.viewType = type;
    create_info.format = format;
    create_info.subresourceRange.aspectMask = aspect;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;
}

VkResult create_image2d(VkDevice device, VkFormat format, const VkExtent2D& extent, VkImageUsageFlags usage, VkImage& image)
{
	CVK_ASSERT(device != VK_NULL_HANDLE);
	return create_image(device, VK_IMAGE_TYPE_2D, format, 1, extent, usage, image);
}   

VkResult create_image_view(VkDevice device, const VkImageViewCreateInfo& create_info, VkImageView& image_view)
{
	CVK_ASSERT(device != VK_NULL_HANDLE);
	return vkCreateImageView(device, &create_info, nullptr, &image_view);
}

VkResult create_image_view(VkDevice device, VkImageViewType type, VkImage image, VkFormat format, uint32_t layers, VkImageAspectFlags image_aspect, VkImageView& image_view)
{
	CVK_ASSERT(device != VK_NULL_HANDLE);
	VkImageViewCreateInfo image_view_create_info = {};
	image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	image_view_create_info.viewType = type;
	image_view_create_info.image = image;
	image_view_create_info.format = format;
	image_view_create_info.subresourceRange.baseMipLevel = 0;
	image_view_create_info.subresourceRange.levelCount = 1;
	image_view_create_info.subresourceRange.baseArrayLayer = 0;
	image_view_create_info.subresourceRange.layerCount = layers;
	image_view_create_info.subresourceRange.aspectMask = image_aspect;

	return create_image_view(device, image_view_create_info, image_view);
}

VkResult create_image_view2d(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags image_aspect, VkImageView& image_view)
{
	CVK_ASSERT(device != VK_NULL_HANDLE);
	return create_image_view(device, VK_IMAGE_VIEW_TYPE_2D, image, format, 1, image_aspect, image_view);
}

// VkResult bind_memory_to_image(VkDevice device, VkImage image, VkDeviceMemory memory)
// {
// 	return vkBindImageMemory(device, image, memory, 0);
// }

void destroy_image(VkDevice device, VkImage image)
{
	CVK_ASSERT(device != VK_NULL_HANDLE);
	CVK_ASSERT(image != VK_NULL_HANDLE);
	vkDestroyImage(device, image, nullptr);
}

void destroy_image_view(VkDevice device, VkImageView image_view)
{
	CVK_ASSERT(device != VK_NULL_HANDLE);
	CVK_ASSERT(image_view != VK_NULL_HANDLE);
	vkDestroyImageView(device, image_view, nullptr);
}

CVK_API void get_default_sampler_create_info(VkSamplerCreateInfo& create_info)
{
    create_info = {};

    create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    create_info.pNext = NULL;
    create_info.magFilter = VK_FILTER_NEAREST;
    create_info.minFilter = VK_FILTER_NEAREST;
    create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    create_info.mipLodBias = 0.0f;
    create_info.anisotropyEnable = VK_FALSE;
    create_info.maxAnisotropy = 1;
    create_info.compareOp = VK_COMPARE_OP_NEVER;
    create_info.minLod = 0.0f;
    create_info.maxLod = 0.0f;
    create_info.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    create_info.unnormalizedCoordinates = VK_FALSE;
}

CVK_API VkResult create_sampler(VkDevice device, VkSamplerCreateInfo CONST_REFERENCE create_info, VkSampler& sampler)
{
	CVK_ASSERT(device != VK_NULL_HANDLE);
	return vkCreateSampler(device, &create_info, nullptr, &sampler);
}

CVK_API void destroy_sampler(VkDevice device, VkSampler sampler)
{
	CVK_ASSERT(device != VK_NULL_HANDLE);
	CVK_ASSERT(sampler != VK_NULL_HANDLE);
	vkDestroySampler(device, sampler, nullptr);
}

};