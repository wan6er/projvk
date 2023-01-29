#pragma once

#include "image_view.h"
#include "cvk/base/base_memorized.h"

namespace cvk
{
    
    template<class _ImageViewBase>
    class MemorizedImageView : public BaseMemorized<_ImageViewBase>
    {
    public:
        MemorizedImageView(VkDevice device);
        virtual ~MemorizedImageView();

        template<class...__Args>
        VkResult create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property, VkImageAspectFlags aspect, __Args&&...args);

    protected:
        using _ImageViewBase::create_image_view;
        // using BaseMemorized<_ImageViewBase>::create;
    };

    
    template<class _ImageViewBase, VkMemoryPropertyFlags _MemoryProperty, VkImageAspectFlags _Aspect>
    class BaseTypeMemorizedImageView : public BaseTypeMemorized<_ImageViewBase, _MemoryProperty>
    {
    public:
        BaseTypeMemorizedImageView(VkDevice device) : BaseTypeMemorized<_ImageViewBase, _MemoryProperty>(device) 
        {
            this->set_subresource_aspect(_Aspect);
        }
        virtual ~BaseTypeMemorizedImageView() = default;

        template<class...__Args>
        VkResult create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, __Args&&...args);

    protected:
        using _ImageViewBase::create_image_view;
        // using BaseMemorized<_ImageViewBase>::create;
    };

} // namespace cvk

#include "memorized_image_view.inl"