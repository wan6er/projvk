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

// queue family
VkResult get_queue_family_properties(VkPhysicalDevice physical_device, std::vector<VkQueueFamilyProperties>& properties) 
{
    return query_properties(vkGetPhysicalDeviceQueueFamilyProperties, properties, physical_device);
}

VkResult get_queue_family_index_present_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface, uint32_t index, VkBool32 &support)
{
    return vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, index, surface, &support);
}

void get_queue_family_index(VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkQueueFamilyProperties> CONST_REFERENCE properties, VkQueueFlagBits queue_flag, uint32_t& index, uint32_t& present_index) 
{
    index = UINT32_MAX;
    present_index = UINT32_MAX;
    
    // std::vector<VkBool32> supports(properties.size());
    for (uint32_t i = 0; i < properties.size(); ++i) {
        if (properties[i].queueFlags & queue_flag) {
            index = i;
            VkBool32 both_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &both_support);
            if (both_support == VK_TRUE) {
                present_index = i;
            }
            break;
        }
    }
    if (present_index == UINT32_MAX) {
        for (uint32_t i = 0; i < properties.size(); ++i) {
            VkBool32 present_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &present_support);
            if (present_support == VK_TRUE) {
                present_index = i;
            }
        }

    }
}

void get_queue_family_index(std::vector<VkQueueFamilyProperties> CONST_REFERENCE properties, VkQueueFlagBits queue_flag, uint32_t &index, uint32_t offset)
{    
    index = UINT32_MAX;
    
    for (uint32_t i = offset; i < properties.size(); ++i) {
        if (properties[i].queueFlags & queue_flag) {
            index = i;
            break;
        }
    }
}

CVK_API void get_queue_family_index(VkPhysicalDevice physical_device, VkQueueFlagBits queue_flag, uint32_t& index, uint32_t offset)
{
    std::vector<VkQueueFamilyProperties> properties;
    get_queue_family_properties(physical_device, properties);
    return get_queue_family_index(properties, queue_flag, index, offset);
}

CVK_API void get_queue_family_index_present_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkQueueFlagBits queue_flag, uint32_t& index, uint32_t offset)
{
    std::vector<VkQueueFamilyProperties> properties;
    get_queue_family_properties(physical_device, properties);
    for (uint32_t i = offset; i < properties.size(); ++i) {
        get_queue_family_index(properties, queue_flag, i, i);
        VkBool32 supported = VK_FALSE;
        get_queue_family_index_present_support(physical_device, surface, i, supported);
        if (supported == VK_TRUE) {
            index = i;
            break;
        }
    }
}


};