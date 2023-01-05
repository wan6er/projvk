#pragma once

#include "cvk/initialize/image_initialize.h"
#include "cvk/initialize/swapchain_initialize.h"

template<class _BaseImage>
template<class...__Args>
cvk::ImageView<_BaseImage>::ImageView(VkImageAspectFlags aspect, __Args&&...args) :
    utils::BaseObj<VkImageView>(),
    _BaseImage(std::forward<__Args>(args)...)
{    
    VkImageCreateInfo info = _BaseImage::image_info();
    __cvk::get_default_image_view_create_info(info.format, static_cast<VkImageViewType>(info.imageType), aspect, _BaseImage::object(), _create_info);
}

template<class _BaseImage>
template<class...__Args>
cvk::ImageView<_BaseImage>::ImageView(const VkImageViewCreateInfo& info, __Args&&...args) :
    utils::BaseObj<VkImageView>(),
    _BaseImage(std::forward<__Args>(args)...)
{    
    image_view_info() = info;
}

template<class _BaseImage>
template<class...__Args>
cvk::ImageView<_BaseImage>::ImageView(const VkSwapchainCreateInfoKHR& info, __Args&&...args) :
    utils::BaseObj<VkImageView>(),
    _BaseImage(std::forward<__Args>(args)...)
{    
    __cvk::get_swapchain_image_view_create_info(info, image_view_info());
}

// template<class _BaseImage>
// cvk::ImageView<_BaseImage>::ImageView(const ImageView& image_view) :
//     utils::BaseObj<VkImageView>(image_view),
//     _BaseImage(image_view),
//     _device(image_view._device),
//     _create_info(image_view._create_info)
// {    
// }

template<class _BaseImage>
cvk::ImageView<_BaseImage>::~ImageView()
{
    if (isolated()) {
        release();
    }
}

template<class _BaseImage>
cvk::ImageView<_BaseImage>::operator VkImageView CONST_REFERENCE () const
{
    return object();
}

template<class _BaseImage>
VkResult cvk::ImageView<_BaseImage>::create_image_view(VkDevice device)
{
    _device = device;
    _create_info.image = _BaseImage::object();
    VkResult result = __cvk::create_image_view(_device, _create_info, object());
    return result;
}

template<class _BaseImage>
void cvk::ImageView<_BaseImage>::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::destroy_image_view(_device, object());
    }
}

template<class _BaseImage>
auto cvk::ImageView<_BaseImage>::image_view_info() -> VkImageViewCreateInfo&
{
    return _create_info;
}