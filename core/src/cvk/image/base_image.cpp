#include "cvk/image/base_image.h"
#include "cvk/initialize/image_initialize.h"
#include "cvk/initialize/memory_initialize.h"



namespace cvk
{


// BaseImage::BaseImage(VkDevice device, VkImageType type, VkFormat format, uint32_t layers, const VkExtent2D &extent, VkImageUsageFlags usage) :
//     _device(device)
// {
//     __cvk::create_image(device, type, format, layers, extent, usage, object());
// }

// BaseImage::BaseImage(VkDevice device, VkImageCreateInfo CONST_REFERENCE info) :
//     utils::BaseObj<VkImage>(),
//     _create_info(info),
//     _device(device)
// {
// }

BaseImage::BaseImage(VkDevice device, VkImage image) :
    utils::BaseObj<VkImage>(image)
{
}

BaseImage::BaseImage(VkDevice device) :
    _device(device)
{
}

// BaseImage::BaseImage(const BaseImage& image) :
//     utils::BaseObj<VkImage>(image),
//     utils::ClonedObj(),
//     _device(image.get_device()),
//     _create_info(image._create_info)
// {
// }

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

};
