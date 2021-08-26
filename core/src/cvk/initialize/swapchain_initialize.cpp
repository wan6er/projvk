#include "cvk/initialize/swapchain_initialize.h"

VkResult __cvk::create_swapchain(VkPhysicalDevice physical_device, VkDevice device, VkSurfaceKHR surface, VkPresentModeKHR present_mode, VkSwapchainKHR& swapchain, VkSwapchainKHR old_swapchain)
{
    VkSurfaceFormatKHR surface_format;
    get_device_format(physical_device, surface, surface_format);

	VkSurfaceCapabilitiesKHR surface_capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &surface_capabilities);
    
    VkExtent2D swapchainExtent = {};
	swapchainExtent = surface_capabilities.currentExtent;
    
    uint32_t desiredNumberOfSwapchainImages = surface_capabilities.minImageCount + 1;
	if ((surface_capabilities.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surface_capabilities.maxImageCount))
	{
		desiredNumberOfSwapchainImages = surface_capabilities.maxImageCount;
	}

	// Find the transformation of the surface
	VkSurfaceTransformFlagsKHR transform;
	if (surface_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		// We prefer a non-rotated transform
		transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		transform = surface_capabilities.currentTransform;
	}
    
	VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    VkSwapchainCreateInfoKHR swapchain_create_info = {};
	swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_create_info.surface = surface;
	swapchain_create_info.minImageCount = desiredNumberOfSwapchainImages;
	swapchain_create_info.imageFormat = surface_format.format;
	swapchain_create_info.imageColorSpace = surface_format.colorSpace;
	swapchain_create_info.imageExtent = { swapchainExtent.width, swapchainExtent.height };
	swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchain_create_info.preTransform = (VkSurfaceTransformFlagBitsKHR)transform;
	swapchain_create_info.imageArrayLayers = 1;
	swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchain_create_info.queueFamilyIndexCount = 0;
	swapchain_create_info.presentMode = present_mode;
	// Setting oldSwapChain to the saved handle of the previous swapchain aids in resource reuse and makes sure that we can still present already acquired images
	swapchain_create_info.oldSwapchain = old_swapchain;
	// Setting clipped to VK_TRUE allows the implementation to discard rendering outside of the surface area
	swapchain_create_info.clipped = VK_TRUE;
	swapchain_create_info.compositeAlpha = compositeAlpha;

	// Enable transfer source on swap chain images if supported
	if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
		swapchain_create_info.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	}

	// Enable transfer destination on swap chain images if supported
	if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
		swapchain_create_info.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	}

	return vkCreateSwapchainKHR(device, &swapchain_create_info, nullptr, &swapchain);
}

void __cvk::destroy_swapchain(VkDevice device, VkSwapchainKHR swapchain)
{
	vkDestroySwapchainKHR(device, swapchain, nullptr);
}

void __cvk::get_all_swapchain_image(VkDevice device, VkSwapchainKHR swapchain, std::vector<VkImage>& images)
{
    uint32_t count;
	vkGetSwapchainImagesKHR(device, swapchain, &count, NULL);
	images.resize(count);
	vkGetSwapchainImagesKHR(device, swapchain, &count, images.data());
}

void __cvk::check_queue_family_present_available(VkPhysicalDevice device, VkSurfaceKHR surface, uint32_t index, VkBool32 &available)
{
	vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface, &available);
}

void __cvk::get_all_device_formats(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR> &formats)
{
	uint32_t count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr);
	formats.resize(count);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, formats.data());
}

void __cvk::get_all_present_modes(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR>& present_modes)
{
	uint32_t count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, NULL);
	present_modes.resize(count);
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, present_modes.data());
}

void __cvk::get_device_format(VkPhysicalDevice device, VkSurfaceKHR surface, VkSurfaceFormatKHR &surface_format, VkFormat format)
{
	std::vector<VkSurfaceFormatKHR> formats;
	__cvk::get_all_device_formats(device, surface, formats);

	if ((formats.size() == 1) && (formats[0].format == VK_FORMAT_UNDEFINED))
	{
		surface_format.format = format;
		surface_format.colorSpace = formats[0].colorSpace;
		return;
	}
	for (auto &item : formats)
	{
		if (item.format == format) 
		{
			surface_format = item;
			break;
		}
	}
}
