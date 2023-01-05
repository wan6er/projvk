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
        explicit ImageView(VkImageAspectFlags aspect, __Args&&...args);
        template<class...__Args>
        explicit ImageView(const VkImageViewCreateInfo& info, __Args&&...args);
        template<class...__Args>
        explicit ImageView(const VkSwapchainCreateInfoKHR& info, __Args&&...args);

        // ImageView(VkDevice device);
        // ImageView(const ImageView& image_view) = default;
        ImageView(ImageView CONST_REFERENCE image_view) = default;
        ~ImageView();

        operator VkImageView CONST_REFERENCE () const;

        VkResult create_image_view(VkDevice device);
        auto image_view_info() -> VkImageViewCreateInfo&;

    protected:
        void release();
        
    private:
        VkImageViewCreateInfo _create_info;
        VkDevice _device = VK_NULL_HANDLE;
    };

    typedef ImageView<Image2D> ImageView2D;
};

#include "image_view.inl"