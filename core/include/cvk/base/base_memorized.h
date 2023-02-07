#pragma once

#include "cvk/vk_header.h"
#include "cvk/memory.h"

#include <type_traits>

namespace cvk
{

static VkMemoryPropertyFlags const MEMORY_STANDARD = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
static VkMemoryPropertyFlags const MEMORY_WRITABLE = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
static VkMemoryPropertyFlags const MEMORY_READABLE = MEMORY_WRITABLE | VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
static VkMemoryPropertyFlags const MEMORY_WRITABLE_FULL_SPEED = MEMORY_WRITABLE | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    
    template<class _Base>
    class BaseMemorized : 
        public _Base,
        public Memory
    {
    public:
        BaseMemorized(VkDevice device);
        virtual ~BaseMemorized() = default;

        template<class...__Args>
        VkResult create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property, __Args&&...args);

    protected:
        // VkResult allocate_bind(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property);
        
        using _Base::create;
        using Memory::allocate;
    };

    template<class _Base, VkMemoryPropertyFlags _Type>
    class BaseTypeMemorized : 
        public BaseMemorized<_Base>
    {
    public:
        BaseTypeMemorized(VkDevice device) : BaseMemorized<_Base>(device) {}
        virtual ~BaseTypeMemorized() = default;

        template<class...__Args>
        VkResult create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, __Args&&...args)
        {
            return BaseMemorized<_Base>::create(properties, _Type, std::forward<__Args>(args)...);
        }

    protected:
        // using BaseMemorized<_Base>::create;

    };

} // namespace cvk

#include "base_memorized.inl"