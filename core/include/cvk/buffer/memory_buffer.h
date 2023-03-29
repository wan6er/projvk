#pragma once

#include "base_buffer.h"

#include "cvk/base/base_memorized.h"

namespace cvk
{
    class MemoryBuffer : public cvk::BaseMemorized<Buffer>
    {
    public:
        using cvk::BaseMemorized<Buffer>::BaseMemorized;

        inline VkResult create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property, uint32_t size, VkBufferUsageFlags usage)
        {
            return cvk::BaseMemorized<Buffer>::create(properties, property, size, usage);
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