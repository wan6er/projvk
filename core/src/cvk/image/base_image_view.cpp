#include "cvk/image/base_image_view.h"

#include "cvk/initialize/image_initialize.h"

namespace cvk::basic
{

ImageView::ImageView(VkDevice device) :
    _device(device)
{
    __cvk::get_default_image_view_create_info(_create_info);
}

ImageView::~ImageView()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

void ImageView::setup(VkFormat format, VkImageViewType type, VkImageAspectFlags aspect)
{
    set_subresource_aspect(aspect);
    __cvk::get_image_view_create_info(format, type, VK_NULL_HANDLE, get_subresource_range(), _create_info);
}

void ImageView::setup(VkFormat format)
{
    _create_info.format = format;
}

VkResult ImageView::create(VkImage image)
{
    _create_info.image = image;
    _create_info.subresourceRange = this->get_subresource_range();
    return __cvk::create_image_view(_device, _create_info, object());
}

VkResult ImageView::create(VkImageViewCreateInfo CONST_REFERENCE info)
{
    _create_info = info;
    return __cvk::create_image_view(_device, _create_info, object());
}

auto ImageView::get_descriptor_info(VkImageLayout layout, VkSampler sampler) const -> VkDescriptorImageInfo
{
    VkDescriptorImageInfo descriptor_info = {};
    descriptor_info.sampler = sampler;
    descriptor_info.imageView = object();
    descriptor_info.imageLayout = layout;
    return descriptor_info;
}

void ImageView::release()
{
    if (object() != VK_NULL_HANDLE) {
        __cvk::destroy_image_view(_device, object());
    }
}

} // namespace cvk

