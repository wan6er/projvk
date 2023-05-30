#pragma once

#include "memory_buffer.h"

namespace cvk
{

    template<class _Buffer, class _Memory>
    class BaseMemoryAddressBuffer : public BaseMemoryBuffer<_Buffer, _Memory>
    {
        using _Base = BaseMemoryBuffer<_Buffer, _Memory>;

    public:
        using _Base::_Base;

        uint64_t get_address()
        {
            uint64_t addr {};
            __cvk::get_buffer_address(_Buffer::get_device(), _Buffer::object(), addr);
            return addr;
        }

    };

    using MemoryAddressBuffer = BaseMemoryAddressBuffer<Buffer, Memory>;
    
    template<class _Buffer, class _Memory, VkMemoryPropertyFlags _Property, VkBufferUsageFlags _Usage>
    class BaseTypeMemoryAddressBuffer : public BaseMemoryAddressBuffer<_Buffer, _Memory>
    {
        using BaseMemoryAddressBufferType = BaseMemoryAddressBuffer<_Buffer, _Memory>;

    public:
        using BaseMemoryAddressBufferType::BaseMemoryAddressBufferType;

        inline VkResult create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, uint32_t size) 
        {
            return BaseMemoryAddressBufferType::create(properties, _Property, size, _Usage);
        }

    };

};