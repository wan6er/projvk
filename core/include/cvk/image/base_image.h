#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"
#include "utils/cloned_obj.h"

#include "subresource.h"

namespace cvk::basic
{

class CVK_API Image : 
    protected utils::BaseObj<VkImage>, 
    virtual public CVK Subresource
{
public:
    Image(VkImage image);
    Image(VkDevice device);
    Image(Image CONST_REFERENCE image) = default;
    virtual ~Image();

    operator VkImage CONST_REFERENCE () const;

    void setup(VkFormat format, VkExtent3D CONST_REFERENCE extent, VkImageType type, VkImageLayout image_layout, VkImageUsageFlags usage, VkImageTiling tiling);
    VkResult create();

    auto get_memory_requirement() const -> VkMemoryRequirements;
    inline auto get_image_info() -> VkImageCreateInfo& { return _create_info; }
    inline auto get_image_format() -> VkFormat& { return _create_info.format; }
    inline auto get_image_extent() -> VkExtent3D& { return _create_info.extent; }
    // auto get_subresource() -> Subresource&;
    // auto get_subresource() const -> Subresource CONST_REFERENCE;

protected:
    auto get_device() const -> VkDevice;
    void release();

private:
    VkImageCreateInfo _create_info;
    VkDevice _device = VK_NULL_HANDLE;
    // bool _no_release = false;
};


};