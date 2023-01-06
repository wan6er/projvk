#include "cvk/swapchain.h"
#include "cvk/initialize/surface_initialize.h"
#include "cvk/initialize/swapchain_initialize.h"
#include "utils/vector_util.h"

namespace cvk
{


Swapchain::Swapchain(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR> CONST_REFERENCE present_modes, VkSurfaceFormatKHR surface_format) :
    utils::BaseObj<VkSwapchainKHR>(),
    _device(device)
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

VkResult Swapchain::create()
{
    VkResult result = __cvk::create_swapchain_by_info(_device, info(), object());
    __cvk::get_swapchain_images(_device, object(), get_attachments());
    return result;
    // return VkResult();
}

uint32_t Swapchain::acquire(VkSemaphore signal_semaphore, VkFence signal_fence)
{
    VkResult result = __cvk::swapchain_acquire_next_image(_device, object(), _cur_present_index, signal_semaphore, signal_fence);
    CVK_ASSERT(result == VK_SUCCESS);
    return _cur_present_index;
}

VkResult Swapchain::present(VkQueue queue, std::vector<VkSemaphore> CONST_REFERENCE wait)
{
    CVK_ASSERT(_cur_present_index != UINT32_MAX);
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

};