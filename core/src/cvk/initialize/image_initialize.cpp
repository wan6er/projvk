#include "cvk/initialize/image_initialize.h"

VkResult __cvk::create_image(VkDevice device, VkImageType type, VkFormat format, uint32_t layers, const VkExtent2D &extent, VkImageUsageFlags usage, VkImage &image)
{
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

	return vkCreateImage(device, &image_create_info, nullptr, &image);
}

VkResult __cvk::create_image2d(VkDevice device, VkFormat format, const VkExtent2D& extent, VkImageUsageFlags usage, VkImage& image)
{
	return create_image(device, VK_IMAGE_TYPE_2D, format, 1, extent, usage, image);
}   

VkResult __cvk::create_image_view2d(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags image_aspect, VkImageView& image_view)
{
	VkImageViewCreateInfo image_view_create_info = {};
	image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	image_view_create_info.image = image;
	image_view_create_info.format = format;
	image_view_create_info.subresourceRange.baseMipLevel = 0;
	image_view_create_info.subresourceRange.levelCount = 1;
	image_view_create_info.subresourceRange.baseArrayLayer = 0;
	image_view_create_info.subresourceRange.layerCount = 1;
	image_view_create_info.subresourceRange.aspectMask = image_aspect;

	return vkCreateImageView(device, &image_view_create_info, nullptr, &image_view);
}

VkResult __cvk::bind_memory_to_image(VkDevice device, VkImage image, VkDeviceMemory memory)
{
	return vkBindImageMemory(device, image, memory, 0);
}

void __cvk::destroy_image(VkDevice device, VkImage image)
{
	vkDestroyImage(device, image, nullptr);
}

void __cvk::get_image_memory_requirement(VkDevice device, VkImage texture, VkMemoryRequirements& requirement)
{
	vkGetImageMemoryRequirements(device, texture, &requirement);
}