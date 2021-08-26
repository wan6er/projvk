#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"

namespace cvk
{
    class Image : public utils::BaseObj<VkImage>
    {
    public:
        Image(VkDevice device, VkImageType type, VkFormat format, uint32_t layers, const VkExtent2D& extent, VkImageUsageFlags usage);
        ~Image();

        operator VkImage() const;

        auto get_memory_requirement() const -> VkMemoryRequirements;
    
        virtual void release();

    protected:
        auto get_device() const -> VkDevice;
    
    private:
        VkDevice _device;
    };
};