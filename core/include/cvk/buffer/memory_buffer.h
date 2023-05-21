#pragma once

#include "base_buffer.h"

#include "cvk/base/base_memorized.h"

namespace cvk
{

    template<class _Buffer, class _Memory>
    class BaseMemoryBuffer : public BaseMemorized<_Buffer, _Memory>
    {
        using _Base = BaseMemorized<_Buffer, _Memory>;

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

    using MemoryBuffer = BaseMemoryBuffer<Buffer, Memory>;
    
    template<class _Buffer, class _Memory, VkMemoryPropertyFlags _Property, VkBufferUsageFlags _Usage>
    class BaseTypeMemoryBuffer : public BaseMemoryBuffer<_Buffer, _Memory>
    {
        using BaseMemoryBufferType = BaseMemoryBuffer<_Buffer, _Memory>;

    public:
        using BaseMemoryBufferType::BaseMemoryBufferType;

        inline VkResult create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, uint32_t size) 
        {
            return BaseMemoryBufferType::create(properties, _Property, size, _Usage);
        }

    };

};