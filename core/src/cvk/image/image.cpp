#include "cvk/image/image.h"

#include "cvk/initialize/image_initialize.h"

cvk::Image::Image(VkDevice device, VkImageType type, VkFormat format, uint32_t layers, const VkExtent2D &extent, VkImageUsageFlags usage) :
    _device(device)
{
    __cvk::create_image(device, type, format, layers, extent, usage, object());
}

cvk::Image::~Image()
{

}

cvk::Image::operator VkImage() const
{
    return object();
}

void cvk::Image::release()
{
    __cvk::destroy_image(_device, object());
}

auto cvk::Image::get_memory_requirement() const -> VkMemoryRequirements
{
    VkMemoryRequirements requirement;
    __cvk::get_image_memory_requirement(_device, object(), requirement);
    return requirement;
}

auto cvk::Image::get_device() const -> VkDevice
{
    return _device;
}