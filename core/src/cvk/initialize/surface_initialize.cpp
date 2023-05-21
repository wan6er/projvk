#include "cvk/initialize/surface_initialize.h"

#if defined(min)
#undef min
#endif
#if defined(max)
#undef max
#endif

namespace __cvk
{


void destroy_surface(VkInstance instance, VkSurfaceKHR surface)
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
}

VkResult get_surface_formats(VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR>& formats) {
    return query_properties(vkGetPhysicalDeviceSurfaceFormatsKHR, formats, physical_device, surface);
}

VkResult get_surface_capabilities(VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR &capabilities)
{
    return vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &capabilities);
}

VkResult get_surface_present_modes(VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR> &present_modes)
{
    return query_properties(vkGetPhysicalDeviceSurfacePresentModesKHR, present_modes, physical_device, surface);
}

CVK_API VkResult get_surface_info(VkPhysicalDevice physical_device, VkSurfaceKHR surface, uint32_t *width, uint32_t *height, uint32_t *image_count, VkCompositeAlphaFlagBitsKHR *composite_alpha, VkSurfaceTransformFlagBitsKHR *transform)
{    
    VkSurfaceCapabilitiesKHR capabilities;
    VkResult result = get_surface_capabilities(physical_device, surface, capabilities);
    if (result == VK_SUCCESS) {
        if (width != nullptr) {
            *width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, *width));
        }
        if (height != nullptr) {
            *height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, *height));
        }
        if (image_count != nullptr) {
            *image_count = std::max(capabilities.minImageCount, std::min(capabilities.maxImageCount, *image_count));
        }
        if (composite_alpha != nullptr) {
            if (~capabilities.supportedCompositeAlpha & *composite_alpha) {
                *composite_alpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            }
        }
        if (transform != nullptr) {
            if (~capabilities.supportedTransforms & *transform) {
                *transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
            }
        }
    }
    return result;
}

};