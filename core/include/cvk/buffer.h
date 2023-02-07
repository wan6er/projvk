#pragma once

#include "utils/base_obj.h"
#include "utils/base_multi_attach.h"
#include "vk_header.h"

namespace cvk
{
    class CVK_API Buffer : 
        protected utils::BaseObj<VkBuffer>,
        public utils::BaseMultipleAttaches<uint32_t>
    {
    public:
        Buffer(VkDevice device);
        Buffer(VkBuffer buffer);
        Buffer(Buffer CONST_REFERENCE buffer) = default;
        virtual ~Buffer();

        auto get_memory_requirement() const -> VkMemoryRequirements;
        auto get_descriptor_info() const -> VkDescriptorBufferInfo;
        operator VkBuffer() const;
        auto info() -> VkBufferCreateInfo&;

        virtual VkResult create(uint32_t size, VkBufferUsageFlags usage);

    protected:
        void release();
    
    private:
        VkDevice _device = VK_NULL_HANDLE;
        VkBufferCreateInfo _info;
    };

    template<VkBufferUsageFlags _Usage>
    class BaseTypeBuffer : public Buffer
    {
    public:
        using Buffer::Buffer;

        virtual VkResult create(uint32_t size) 
        {
            return Buffer::create(size, _Usage);
        }

    protected:
        using Buffer::create;
    };

};