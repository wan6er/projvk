#pragma once

#include "utils/base_obj.h"
#include "vk_header.h"

namespace cvk
{
    class CVK_API Buffer : protected utils::BaseObj<VkBuffer>
    {
    public:
        Buffer(uint32_t size, VkBufferUsageFlags usage);
        Buffer(VkBuffer buffer);
        Buffer(Buffer CONST_REFERENCE buffer) = default;
        ~Buffer();

        auto get_memory_requirement() const -> VkMemoryRequirements;
        operator VkBuffer() const;
        auto info() -> VkBufferCreateInfo&;

        VkResult create(VkDevice device);

    protected:
        void release();
    
    private:
        VkDevice _device = VK_NULL_HANDLE;
        VkBufferCreateInfo _info;
    };
};