#pragma once

#include "cvk/initialize/image_initialize.h"
#include "cvk/initialize/swapchain_initialize.h"

// template<class _BaseImage>
// template<class...__Args>
// ImageView<_BaseImage>::ImageView(VkDevice device, VkImageAspectFlags aspect, __Args&&...args) :
//     utils::BaseObj<VkImageView>(),
//     _BaseImage(device, std::forward<__Args>(args)...),
//     _device(device)
// {    
//     VkImageCreateInfo info = _BaseImage::image_info();
//     __cvk::get_default_image_view_create_info(info.format, static_cast<VkImageViewType>(info.imageType), aspect, _BaseImage::object(), _create_info);
// }

namespace cvk
{

// template<class _BaseImage>
// template<class...__Args>
// ImageView<_BaseImage>::ImageView(VkDevice device, __Args&&...args):
//     utils::BaseObj<VkImageView>(),
//     _BaseImage(device, std::forward<__Args>(args)...),
//     _device(device)
// {
//     __cvk::get_default_image_view_create_info(_create_info);
// }

// template<class _BaseImage>
// ImageView<_BaseImage>::~ImageView()
// {
//     if (isolated()) {
//         release();
//     }
// }

// template<class _BaseImage>
// ImageView<_BaseImage>::operator VkImageView CONST_REFERENCE () const
// {
//     return object();
// }

// template<class _BaseImage>
// VkResult ImageView<_BaseImage>::create_image_view()
// {
//     _create_info.image = _BaseImage::object();
//     VkImageCreateInfo info = _BaseImage::get_image_info();
    
//     CVK_ASSERT(_create_info.image != VK_NULL_HANDLE);
//     CVK_ASSERT(info.sType == VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO);
    
//     __cvk::get_image_view_create_info(info.format, static_cast<VkImageViewType>(info.imageType), _BaseImage::object(), this->get_all_subresource(), _create_info);

//     return __cvk::create_image_view(_device, _create_info, object());
// }

// template<class _BaseImage>
// VkResult ImageView<_BaseImage>::create_image_view(VkImageAspectFlags aspect)
// {
//     this->set_subresource_aspect(aspect);
//     return create_image_view();
// }

// template<class _BaseImage>
// VkResult ImageView<_BaseImage>::create_image_view(VkImageViewCreateInfo CONST_REFERENCE info)
// {
//     _create_info = info;
//     if (_create_info.image == VK_NULL_HANDLE) {
//         _create_info.image = _BaseImage::object();
//     }
//     CVK_ASSERT(_create_info.image != VK_NULL_HANDLE);
//     return __cvk::create_image_view(_device, _create_info, object());
// }

// template<class _BaseImage>
// VkResult ImageView<_BaseImage>::create_image_view(VkSwapchainCreateInfoKHR CONST_REFERENCE info)
// {
//     __cvk::get_swapchain_image_view_create_info(info, _create_info);
//     _create_info.image = _BaseImage::object();
//     CVK_ASSERT(_create_info.image != VK_NULL_HANDLE);
//     return __cvk::create_image_view(_device, _create_info, object());
// }

// template<class _BaseImage>
// void ImageView<_BaseImage>::release()
// {
//     if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
//         __cvk::destroy_image_view(_device, object());
//     }
// }

// template<class _BaseImage>
// auto ImageView<_BaseImage>::get_image_view_info() -> VkImageViewCreateInfo&
// {
//     return _create_info;
// }

// template<class _BaseImage>
// auto ImageView<_BaseImage>::get_descriptor_info(VkSampler sampler, VkImageLayout layout) const -> VkDescriptorImageInfo
// {
//     VkDescriptorImageInfo descriptor_info = {};
//     descriptor_info.sampler = sampler;
//     descriptor_info.imageView = object();
//     descriptor_info.imageLayout = layout;
//     return descriptor_info;
// }

// template<class _BaseImage>
// auto ImageView<_BaseImage>::get_subresource() const -> VkImageSubresourceRange
// {
//     VkImageSubresourceRange subresource = {};
//     subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//     subresource.baseMipLevel
//     subresource.baseArrayLayer = 0;
//     subresource.layerCount = 1;
// }

} // namespace cvk
