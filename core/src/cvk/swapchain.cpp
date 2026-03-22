#include "cvk/swapchain.h"
#include "cvk/initialize/surface_initialize.h"
#include "cvk/initialize/swapchain_initialize.h"
#include "utils/vector_util.h"

namespace cvk
{


Swapchain::Swapchain(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR> CONST_REFERENCE present_modes, VkSurfaceFormatKHR surface_format) :
    utils::BaseObj<VkSwapchainKHR>(),
    _device(device),
    _physical_device(physical_device),
    _surface(surface),
    _present_modes(present_modes),
    _surface_format(surface_format)
{
    init_info(physical_device, surface, present_modes, surface_format);
}

Swapchain::Swapchain(VkDevice device, VkSwapchainCreateInfoKHR CONST_REFERENCE create_info) :
    _device(device)
{
    info() = create_info;
}

Swapchain::~Swapchain()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

void Swapchain::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) 
    {
        __cvk::destroy_swapchain(_device, object());
    }
}

VkSwapchainCreateInfoKHR &Swapchain::info()
{
    return _create_info;
}

VkImageViewCreateInfo Swapchain::get_image_view_info() const
{
    VkImageViewCreateInfo return_info;
    __cvk::get_swapchain_image_view_create_info(_create_info, return_info);
    return return_info;
}

VkFormat Swapchain::get_format() const
{
    return _create_info.imageFormat;
}

VkExtent2D Swapchain::get_extent() const noexcept
{
    return _create_info.imageExtent;
}

VkResult Swapchain::create()
{
    VkResult result = __cvk::create_swapchain_by_info(_device, info(), object());
    if (result != VK_SUCCESS) {
        return result;
    }

    std::vector<VkImage>& images = get_attachments();
    images.clear();
    __cvk::get_swapchain_images(_device, object(), images);
    _cur_present_index = UINT32_MAX;
    return result;
}

VkResult Swapchain::recreate()
{
    if (_physical_device != VK_NULL_HANDLE && _surface != VK_NULL_HANDLE) {
        VkResult refresh_result = refresh_info();
        if (refresh_result != VK_SUCCESS) {
            return refresh_result;
        }
    }
    return recreate(_create_info);
}

VkResult Swapchain::recreate(VkSwapchainCreateInfoKHR CONST_REFERENCE create_info)
{
    VkSwapchainKHR old_swapchain = object();
    VkSwapchainCreateInfoKHR recreate_info = create_info;
    recreate_info.oldSwapchain = old_swapchain;

    VkSwapchainKHR new_swapchain = VK_NULL_HANDLE;
    VkResult result = __cvk::create_swapchain_by_info(_device, recreate_info, new_swapchain);
    if (result != VK_SUCCESS) {
        return result;
    }

    std::vector<VkImage> images;
    result = __cvk::get_swapchain_images(_device, new_swapchain, images);
    if (result != VK_SUCCESS) {
        __cvk::destroy_swapchain(_device, new_swapchain);
        return result;
    }

    if (old_swapchain != VK_NULL_HANDLE) {
        __cvk::destroy_swapchain(_device, old_swapchain);
    }

    object() = new_swapchain;
    _create_info = recreate_info;
    _create_info.oldSwapchain = VK_NULL_HANDLE;
    get_attachments() = std::move(images);
    _cur_present_index = UINT32_MAX;
    return VK_SUCCESS;
}

VkResult Swapchain::acquire(uint32_t& image_index, VkSemaphore signal_semaphore, VkFence signal_fence)
{
    image_index = UINT32_MAX;
    VkResult result = __cvk::swapchain_acquire_next_image(_device, object(), image_index, signal_semaphore, signal_fence);
    if (result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR) {
        _cur_present_index = image_index;
    } else {
        _cur_present_index = UINT32_MAX;
    }
    return result;
}

uint32_t Swapchain::acquire(VkSemaphore signal_semaphore, VkFence signal_fence)
{
    uint32_t image_index = UINT32_MAX;
    VkResult result = acquire(image_index, signal_semaphore, signal_fence);
    CVK_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR);
    return image_index;
}

VkResult Swapchain::present(VkQueue queue, std::vector<VkSemaphore> CONST_REFERENCE wait)
{
    if (_cur_present_index == UINT32_MAX) {
        return VK_ERROR_OUT_OF_DATE_KHR;
    }
    return __cvk::swapchain_present(queue, object(), wait, _cur_present_index);
}

auto Swapchain::get_images() const -> std::vector<VkImage> CONST_REFERENCE
{
    CVK_ASSERT(object() != VK_NULL_HANDLE);
    return *this;
}

Swapchain::operator ObjType CONST_REFERENCE () const
{
    CVK_ASSERT(object() != VK_NULL_HANDLE);
    return object();
}

void Swapchain::init_info(VkPhysicalDevice physical_device, VkSurfaceKHR surface, const std::vector<VkPresentModeKHR>& present_modes, VkSurfaceFormatKHR surface_format)
{
	VkResult result = VK_SUCCESS;
	std::vector<VkPresentModeKHR> all_present_modes, best_present_modes;
	result = __cvk::get_surface_present_modes(physical_device, surface, all_present_modes);
	if (result != VK_SUCCESS) {
		return;
	}
	utils::vector_filter(all_present_modes, present_modes, best_present_modes);
	if (best_present_modes.size() == 0) {
		return;
	}

    __cvk::get_default_swapchain_create_info(physical_device, surface, best_present_modes[0], surface_format, _create_info);
}

VkResult Swapchain::refresh_info()
{
    if (_physical_device == VK_NULL_HANDLE || _surface == VK_NULL_HANDLE) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    VkImageUsageFlags image_usage = _create_info.imageUsage;
    init_info(_physical_device, _surface, _present_modes, _surface_format);
    if (_create_info.sType != VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }
    _create_info.imageUsage |= image_usage;
    return VK_SUCCESS;
}

};