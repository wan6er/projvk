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
        // template<class...__Args>
        // explicit ImageView(VkDevice device, VkImageAspectFlags aspect, __Args&&...args);
        // template<class...__Args>
        // explicit ImageView(VkDevice device, const VkImageViewCreateInfo& info, __Args&&...args);
        // template<class...__Args>
        // explicit ImageView(VkDevice device, const VkSwapchainCreateInfoKHR& info, __Args&&...args);

        template<class...__Args>
        explicit ImageView(VkDevice device, __Args&&...args);

        // ImageView(VkDevice device);
        // ImageView(const ImageView& image_view) = default;
        ImageView(ImageView CONST_REFERENCE image_view) = default;
        ~ImageView();

        operator VkImageView CONST_REFERENCE () const;

        VkResult create_image_view(VkImageAspectFlags aspect);
        VkResult create_image_view(VkImageViewCreateInfo CONST_REFERENCE info);
        VkResult create_image_view(VkSwapchainCreateInfoKHR CONST_REFERENCE info);

        auto image_view_info() -> VkImageViewCreateInfo&;

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