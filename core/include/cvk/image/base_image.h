#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"
#include "utils/cloned_obj.h"

#include "subresource.h"

namespace cvk
{
    class CVK_API BaseImage : 
        protected utils::BaseObj<VkImage>, 
        public Subresource
    {
    public:
        BaseImage(VkDevice device, VkImage image);
        BaseImage(VkDevice device);
        BaseImage(BaseImage CONST_REFERENCE image) = default;
        virtual ~BaseImage();

        operator VkImage CONST_REFERENCE () const;

        auto get_memory_requirement() const -> VkMemoryRequirements;
        auto get_image_info() -> VkImageCreateInfo&;
        auto get_image_format() const -> VkFormat CONST_REFERENCE;
        auto get_image_extent() const -> VkExtent3D CONST_REFERENCE;
        auto get_subresource() -> Subresource&;
        auto get_subresource() const -> Subresource CONST_REFERENCE;
    
    protected:
        auto get_device() const -> VkDevice;
        void release();
    
    private:
        VkImageCreateInfo _create_info;
        VkDevice _device = VK_NULL_HANDLE;
        // bool _no_release = false;
    };
};