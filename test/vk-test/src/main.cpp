#include <iostream>
#include <cassert>
#include <memory>

#include "cvk.h"
#include "win32/win.h"
#include "utils/base_obj.h"

int main(int argv, char* args[])
{
    uint32_t win_width = 1280;
    uint32_t win_height = 728;
    Windows win("projvk", win_width, win_height);

    std::vector<std::string> instance_extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };
    std::vector<std::string> instance_layers = {
        "VK_LAYER_KHRONOS_validation"
    };
    std::vector<std::string> device_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        // VK_EXT_DEBUG_MARKER_EXTENSION_NAME
    };

    VkResult result;

    cvk::Instance instance(instance_extensions, instance_layers);
    
    std::vector<VkPhysicalDevice>&& devices = instance.get_all_physical_device();
    auto physical_device = devices[0];

    VkPhysicalDeviceFeatures device_features = {};
    cvk::Device device(physical_device, device_extensions, device_features, VK_QUEUE_GRAPHICS_BIT);

    const auto& memory_properties = device.get_memory_properties();

    VkCommandPool graphics_command_pool;
    result = __cvk::create_command_pool(device, device.get_queue_family_index(VK_QUEUE_GRAPHICS_BIT), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, graphics_command_pool);

    cvk::SurfaceWin32 surface(instance, win.instance(), win);

    VkBool32 present_support;
    __cvk::check_queue_family_present_available(physical_device, surface, device.get_queue_family_index(VK_QUEUE_GRAPHICS_BIT), present_support);
    assert(present_support);

    cvk::Swapchain swapchain(physical_device, device, surface);

    std::vector<VkImage> images;
    __cvk::get_all_swapchain_image(device, swapchain, images);

    std::vector<VkCommandBuffer> command_buffers;
    __cvk::create_command_buffers(device, graphics_command_pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, images.size(), command_buffers);

    cvk::Image2D depth_image(device, VK_FORMAT_D32_SFLOAT_S8_UINT, {800, 600}, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
    // VkImage depth_image;
    // result = __cvk::create_image2d(device, VK_FORMAT_D32_SFLOAT_S8_UINT, {800, 600}, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, depth_image);

    VkMemoryRequirements depth_image_requirement = depth_image.get_memory_requirement();
    // __cvk::get_image_memory_requirement(device, depth_image, depth_image_requirement);
    uint32_t index;
    __cvk::get_memory_type(device.get_memory_properties(), depth_image_requirement.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, index);
    cvk::Memory memory(device, depth_image_requirement.size, index);

    VkImageView depth_image_view;
    result = __cvk::bind_memory_to_image(device, depth_image, memory);
    result = __cvk::create_image_view2d(device, depth_image, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_IMAGE_USAGE_SAMPLED_BIT, depth_image_view);
    return 0;
}
