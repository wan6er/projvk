#include "cvk/initialize/swapchain_initialize.h"
#include "cvk/initialize/surface_initialize.h"
#include "utils/vector_util.h"



namespace __cvk
{


VkResult create_swapchain(VkPhysicalDevice physical_device, VkDevice device, VkSurfaceKHR surface, VkSwapchainKHR& swapchain, 
        std::vector<VkPresentModeKHR> present_modes,
        VkSurfaceFormatKHR surface_format,
        uint32_t width, uint32_t height,
        uint32_t image_count,
        uint32_t image_array_layers,
        VkImageUsageFlags usage,
        VkSurfaceTransformFlagBitsKHR transform,
        VkCompositeAlphaFlagBitsKHR composite_alpha,
        VkSharingMode sharing_mode,
        uint32_t queue_family_count,
        VkSwapchainKHR old_swapchain
    )
{
	CVK_ASSERT(physical_device != VK_NULL_HANDLE);
	CVK_ASSERT(device != VK_NULL_HANDLE);
	CVK_ASSERT(surface != VK_NULL_HANDLE);
	VkResult result = VK_SUCCESS;
    // std::vector<VkSurfaceFormatKHR> surface_formats;
	// VkResult result = get_surface_formats(physical_device, surface, surface_formats);
	// if (result != VK_SUCCESS) {
	// 	return result;
	// }
	// VkSurfaceFormatKHR surface_format = surface_formats[0];

	// VkPresentModeKHR present_mode;
	std::vector<VkPresentModeKHR> all_present_modes, best_present_modes;
	result = get_surface_present_modes(physical_device, surface, all_present_modes);
	if (result != VK_SUCCESS) {
		return result;
	}
	utils::vector_filter(all_present_modes, present_modes, best_present_modes);
	if (best_present_modes.size() == 0) {
		return VK_NOT_READY;
	}

	result = get_surface_info(physical_device, surface, &width, &height, &image_count, &composite_alpha, &transform);
	if (result != VK_SUCCESS) {
		return result;
	}

    VkSwapchainCreateInfoKHR swapchain_create_info = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = surface,
		.minImageCount = image_count,
		.imageFormat = surface_format.format,
		.imageColorSpace = surface_format.colorSpace,
		.imageExtent = { width, height },
		.imageArrayLayers = image_array_layers,
		.imageUsage = usage,
		.imageSharingMode = sharing_mode,
		.queueFamilyIndexCount = queue_family_count,
		.preTransform = transform,
		.compositeAlpha = composite_alpha,
		.presentMode = best_present_modes[0],
		.clipped = VK_TRUE,
		.oldSwapchain = old_swapchain
	};
	return vkCreateSwapchainKHR(device, &swapchain_create_info, nullptr, &swapchain);
}

VkResult get_default_swapchain_create_info(VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkPresentModeKHR present_mode, VkSurfaceFormatKHR surface_format, VkSwapchainCreateInfoKHR &create_info)
{
	CVK_ASSERT(physical_device != VK_NULL_HANDLE);
	CVK_ASSERT(surface != VK_NULL_HANDLE);
	VkResult result = VK_SUCCESS;
    
    uint32_t image_array_layers = 1;
    VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    VkSharingMode sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
    uint32_t queue_family_count = 0;
    VkSwapchainKHR old_swapchain = nullptr;

	// std::vector<VkPresentModeKHR> all_present_modes, best_present_modes;
	// result = get_surface_present_modes(_physical_device, _surface, all_present_modes);
	// if (result != VK_SUCCESS) {
	// 	return;
	// }
	// utils::filter_vector(all_present_modes, { present_mode }, best_present_modes);
	// if (best_present_modes.size() == 0) {
	// 	return;
	// }

    uint32_t width = 0, height = 0;
    uint32_t image_count = 0;
    VkSurfaceTransformFlagBitsKHR transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    VkCompositeAlphaFlagBitsKHR composite_alpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	result = get_surface_info(physical_device, surface, &width, &height, &image_count, &composite_alpha, &transform);
	if (result != VK_SUCCESS) {
		return result;
	}

    create_info = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = surface,
		.minImageCount = image_count,
		.imageFormat = surface_format.format,
		.imageColorSpace = surface_format.colorSpace,
		.imageExtent = { width, height },
		.imageArrayLayers = image_array_layers,
		.imageUsage = usage,
		.imageSharingMode = sharing_mode,
		.queueFamilyIndexCount = queue_family_count,
		.preTransform = transform,
		.compositeAlpha = composite_alpha,
		.presentMode = present_mode,
		.clipped = VK_TRUE,
		.oldSwapchain = old_swapchain
	};
	return result;
}

void get_swapchain_image_view_create_info(const VkSwapchainCreateInfoKHR &swapchain_create_info, VkImageViewCreateInfo& view_create_info)
{
	view_create_info = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = swapchain_create_info.imageFormat,
		.subresourceRange =
			{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1}
	};
}

VkResult create_swapchain_by_info(VkDevice device, const VkSwapchainCreateInfoKHR &create_info, VkSwapchainKHR &swapchain)
{
	CVK_ASSERT(device != VK_NULL_HANDLE);
	return vkCreateSwapchainKHR(device, &create_info, nullptr, &swapchain);
}

void destroy_swapchain(VkDevice device, VkSwapchainKHR swapchain)
{
	CVK_ASSERT(device != VK_NULL_HANDLE);
	CVK_ASSERT(swapchain != VK_NULL_HANDLE);
	// if (swapchain != VK_NULL_HANDLE) {
		vkDestroySwapchainKHR(device, swapchain, nullptr);
	// }
}

VkResult get_swapchain_images(VkDevice device, VkSwapchainKHR swapchain, std::vector<VkImage>& images)
{
	CVK_ASSERT(device != VK_NULL_HANDLE);
	CVK_ASSERT(swapchain != VK_NULL_HANDLE);
//     uint32_t count;
// 	vkGetSwapchainImagesKHR(device, swapchain, &count, NULL);
// 	images.resize(count);
// 	vkGetSwapchainImagesKHR(device, swapchain, &count, images.data());
	return query_properties(vkGetSwapchainImagesKHR, images, device, swapchain);
}

CVK_API VkResult swapchain_acquire_next_image(VkDevice device, VkSwapchainKHR swapchain, uint32_t& next_index, VkSemaphore wait_semaphore, VkFence wait_fence)
{
	CVK_ASSERT(device != VK_NULL_HANDLE);
	CVK_ASSERT(swapchain != VK_NULL_HANDLE);
	return vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, wait_semaphore, wait_fence, &next_index);
}

CVK_API VkResult swapchain_present(VkQueue queue, VkSwapchainKHR CONST_REFERENCE swapchain, std::vector<VkSemaphore> CONST_REFERENCE wait, uint32_t& next_index)
{
	CVK_ASSERT(queue != VK_NULL_HANDLE);
	VkPresentInfoKHR present = {};
	present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present.pNext = NULL;
	present.swapchainCount = 1;
	present.pSwapchains = &swapchain;
	present.pImageIndices = &next_index;
	present.pResults = NULL;
	utils::vector_fill_info(wait, present.waitSemaphoreCount, present.pWaitSemaphores);
	return vkQueuePresentKHR(queue, &present);
}

};