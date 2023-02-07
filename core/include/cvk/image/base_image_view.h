#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"

#include "subresource.h"

namespace cvk
{

namespace basic
{

class CVK_API ImageView : 
    protected utils::BaseObj<VkImageView>, 
    virtual public CVK Subresource
{
public:
    explicit ImageView(VkDevice device);
    ImageView(ImageView CONST_REFERENCE image_view) = default;
    virtual ~ImageView();

    void setup(VkFormat format, VkImageViewType type, VkImageAspectFlags aspect);
    void setup(VkFormat format);
    VkResult create(VkImage image);
    VkResult create(VkImageViewCreateInfo CONST_REFERENCE info);

    inline operator VkImageView CONST_REFERENCE () const { return object(); }

    inline auto get_image_view_info() -> VkImageViewCreateInfo& { return _create_info; }
    auto get_descriptor_info(VkSampler sampler, VkImageLayout layout) const -> VkDescriptorImageInfo;

protected:
    void release();
    
private:
    VkImageViewCreateInfo _create_info;
    VkDevice _device = VK_NULL_HANDLE;
};

};

template<VkImageViewType _Type, VkImageAspectFlags _Aspect>
class BaseTypeImageView : public basic::ImageView
{
public:
    BaseTypeImageView(VkDevice device) :
        basic::ImageView(device)
    {
        basic::ImageView::setup(VK_FORMAT_UNDEFINED, _Type, _Aspect);
    }
    
    VkResult create(VkFormat format, VkImage image)
    {
        basic::ImageView::setup(format);
        return basic::ImageView::create(image);
    }

};

};
