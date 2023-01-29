#pragma once

#include "cvk/vk_header.h"
#include "cvk/image/image2d.h"
#include "utils/base_obj.h"

#include <type_traits>

namespace cvk
{
    
    template<class _BaseImage>
    class ImageView : protected utils::BaseObj<VkImageView>, 
        public std::enable_if_t<std::is_base_of_v<BaseImage, _BaseImage>, _BaseImage>
    {
    public:
        template<class...__Args>
        explicit ImageView(VkDevice device, __Args&&...args);

        ImageView(ImageView CONST_REFERENCE image_view) = default;
        ~ImageView();

        operator VkImageView CONST_REFERENCE () const;

        VkResult create_image_view();
        VkResult create_image_view(VkImageAspectFlags aspect);
        VkResult create_image_view(VkImageViewCreateInfo CONST_REFERENCE info);
        VkResult create_image_view(VkSwapchainCreateInfoKHR CONST_REFERENCE info);

        auto get_image_view_info() -> VkImageViewCreateInfo&;
        auto get_descriptor_info(VkSampler sampler, VkImageLayout layout) const -> VkDescriptorImageInfo;

    protected:
        using utils::BaseObj<VkImageView>::object;
        using utils::BaseObj<VkImageView>::isolated;
        void release();
        
    private:
        VkImageViewCreateInfo _create_info;
        VkDevice _device = VK_NULL_HANDLE;
    };

    typedef ImageView<Image2D> ImageView2D;
};

#include "image_view.inl"