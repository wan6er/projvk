#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"
#include "utils/cloned_obj.h"

namespace cvk
{
    class CVK_API BaseImage : protected utils::BaseObj<VkImage>
    {
    public:
        // Image(VkDevice device, VkImageType type, VkFormat format, uint32_t layers, const VkExtent2D& extent, VkImageUsageFlags usage);
        // BaseImage(VkDevice device, VkImageCreateInfo CONST_REFERENCE info);        
        BaseImage(VkDevice device, VkImage image);
        BaseImage(VkDevice device);
        BaseImage(BaseImage CONST_REFERENCE image) = default;
        // Image(VkDevice device, VkFormat format, const VkExtent2D& extent, VkImageUsageFlags usage);
        virtual ~BaseImage();

        operator VkImage CONST_REFERENCE () const;

        auto get_memory_requirement() const -> VkMemoryRequirements;
        auto get_image_info() -> VkImageCreateInfo&;
    
    protected:
        auto get_device() const -> VkDevice;
        void release();
    
    private:
        VkImageCreateInfo _create_info;
        VkDevice _device = VK_NULL_HANDLE;
        // bool _no_release = false;
    };
};