#pragma once

#include "cvk/vk_header.h"
#include "cvk/memory.h"

#include <type_traits>

namespace cvk
{

static VkMemoryPropertyFlags const MEMORY_STANDARD = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
static VkMemoryPropertyFlags const MEMORY_WRITABLE = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
static VkMemoryPropertyFlags const MEMORY_READABLE = MEMORY_WRITABLE | VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
static VkMemoryPropertyFlags const MEMORY_WRITABLE_SPEED = MEMORY_WRITABLE | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    
    template<class _Base, class _Memory>
    class BaseMemorized : 
        public _Base,
        public _Memory
    {
    public:
        BaseMemorized(VkDevice device);
        BaseMemorized(VkDevice device, VkDeviceMemory memory);
        virtual ~BaseMemorized() = default;

        template<class...__Args>
        VkResult create_obj(__Args&&...args);

        template<class..._Args>
        VkResult create_mem(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property);

        VkResult bind();

    protected:
        using _Base::create;
        using _Memory::allocate;

    };

    template<class _Base, VkMemoryPropertyFlags _Type>
    class BaseTypeMemorized : 
        public BaseMemorized<_Base, Memory>
    {
        using _Memory = Memory;

    public:
        BaseTypeMemorized(VkDevice device) : BaseMemorized<_Base, _Memory>(device) {}
        virtual ~BaseTypeMemorized() = default;

        template<class...__Args>
        VkResult create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, __Args&&...args);

    protected:
        // using BaseMemorized<_Base>::create;

    };

} // namespace cvk

#include "base_memorized.inl"