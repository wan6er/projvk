#include "cvk/image/base_image.h"
#include "cvk/initialize/image_initialize.h"
#include "cvk/initialize/memory_initialize.h"



namespace cvk
{

BaseImage::BaseImage(VkDevice device, VkImage image) :
    utils::BaseObj<VkImage>(image)
{
    __cvk::get_default_image_create_info(get_image_info());
}

BaseImage::BaseImage(VkDevice device) :
    _device(device)
{
    __cvk::get_default_image_create_info(get_image_info());
}

BaseImage::~BaseImage()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

BaseImage::operator VkImage CONST_REFERENCE () const
{
    CVK_ASSERT(object() != VK_NULL_HANDLE);
    return object();
}

void BaseImage::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::destroy_image(_device, object());
    }
}

auto BaseImage::get_image_info() -> VkImageCreateInfo&
{
    return _create_info;
}

auto BaseImage::get_image_format() const -> VkFormat CONST_REFERENCE
{
    return _create_info.format;
}

auto BaseImage::get_image_extent() const -> VkExtent3D CONST_REFERENCE
{
    return _create_info.extent;
}

auto BaseImage::get_device() const -> VkDevice
{
    return _device;
}

auto BaseImage::get_memory_requirement() const -> VkMemoryRequirements
{
    CVK_ASSERT(_device != VK_NULL_HANDLE);
    VkMemoryRequirements requirement = {};
    __cvk::get_memory_requirement(_device, object(), requirement);
    return requirement;
}

auto BaseImage::get_subresource() -> Subresource&
{
    return *this;
}

auto BaseImage::get_subresource() const -> Subresource CONST_REFERENCE
{
    return *this;
}

};
