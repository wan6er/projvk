#pragma once

#include "base_buffer.h"

#include "cvk/base/base_memorized.h"

namespace cvk
{
    class MemoryBuffer : public BaseMemorized<Buffer, Memory>
    {
        using _Base = BaseMemorized<Buffer, Memory>;

    public:
        using _Base::_Base;

        inline VkResult create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property, uint32_t size, VkBufferUsageFlags usage)
        {
            // return cvk::BaseMemorized<Buffer>::create(properties, property, size, usage);
            VkResult result = VK_SUCCESS;
            if ((result = _Base::create_obj(size, usage))) {
                return result;
            }

            if ((result = _Base::create_mem(properties, property))) {
                return result;
            }

            return _Base::bind();
        }

    };
    
    template<VkMemoryPropertyFlags _Property, VkBufferUsageFlags _Usage>
    class BaseTypeMemoryBuffer : public MemoryBuffer
    {
    public:
        using MemoryBuffer::MemoryBuffer;

        inline VkResult create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, uint32_t size) 
        {
            return MemoryBuffer::create(properties, _Property, size, _Usage);
        }

    };

};