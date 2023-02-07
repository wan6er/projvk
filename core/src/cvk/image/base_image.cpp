#include "cvk/image/base_image.h"
#include "cvk/initialize/image_initialize.h"
#include "cvk/initialize/memory_initialize.h"



namespace cvk::basic
{

Image::Image(VkImage image) :
    utils::BaseObj<VkImage>(image)
{
    __cvk::get_default_image_create_info(_create_info);
}

Image::Image(VkDevice device) :
    _device(device)
{
    __cvk::get_default_image_create_info(_create_info);
}

Image::~Image()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

Image::operator VkImage CONST_REFERENCE () const
{
    CVK_ASSERT(object() != VK_NULL_HANDLE);
    return object();
}

void Image::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::destroy_image(_device, object());
    }
}

void Image::setup(VkFormat format, VkExtent3D CONST_REFERENCE extent, VkImageType type, VkImageLayout image_layout, VkImageUsageFlags usage, VkImageTiling tiling)
{
    __cvk::get_image_create_info(format, extent, type, image_layout, usage, tiling, _create_info);
}

VkResult Image::create()
{
    CVK_ASSERT(object() == VK_NULL_HANDLE);
    _create_info.mipLevels = mip_levels;
    _create_info.arrayLayers = array_layers;
    return __cvk::create_image(get_device(), _create_info, object());
}

auto Image::get_device() const -> VkDevice
{
    return _device;
}

auto Image::get_memory_requirement() const -> VkMemoryRequirements
{
    CVK_ASSERT(_device != VK_NULL_HANDLE);
    VkMemoryRequirements requirement = {};
    __cvk::get_memory_requirement(_device, object(), requirement);
    return requirement;
}

};
