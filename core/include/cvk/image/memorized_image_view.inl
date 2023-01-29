#include "cvk/image/memorized_image_view.h"

namespace cvk
{

template<class _ImageViewBase>
MemorizedImageView<_ImageViewBase>::MemorizedImageView(VkDevice device) :
    BaseMemorized<_ImageViewBase>(device)
{
}

template<class _ImageViewBase>
MemorizedImageView<_ImageViewBase>::~MemorizedImageView()
{
}

template<class _ImageViewBase>
template<class...__Args>
VkResult MemorizedImageView<_ImageViewBase>::create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property, VkImageAspectFlags aspect, __Args&&...args)
{
    VkResult result = VK_SUCCESS;
    result = BaseMemorized<_ImageViewBase>::create(properties, property, std::forward<__Args>(args)...);
    if (result != VK_SUCCESS) {
        goto __RESULT;
    }

    result = _ImageViewBase::create_image_view(aspect);
    CVK_ASSERT(result == VK_SUCCESS);

__RESULT:
    return result;
}


template<class _ImageViewBase, VkMemoryPropertyFlags _MemoryProperty, VkImageAspectFlags _Aspect>
template<class...__Args>
VkResult BaseTypeMemorizedImageView<_ImageViewBase, _MemoryProperty, _Aspect>::create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, __Args&&...args)
{
    VkResult result = VK_SUCCESS;
    result = BaseTypeMemorized<_ImageViewBase, _MemoryProperty>::create(properties, std::forward<__Args>(args)...);
    if (result != VK_SUCCESS) {
        goto __RESULT;
    }

    result = _ImageViewBase::create_image_view();
    CVK_ASSERT(result == VK_SUCCESS);

__RESULT:
    return result;
}


} // namespace cvk
