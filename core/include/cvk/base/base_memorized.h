#pragma once

#include "cvk/vk_header.h"
#include "cvk/memory.h"

#include <type_traits>

namespace cvk
{
    template<class _Base>
    class BaseMemorized : 
        public _Base,
        public Memory
    {
    public:
        BaseMemorized(VkDevice device);
        virtual ~BaseMemorized();

        template<class...__Args>
        VkResult create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property, __Args&&...args);

    protected:
        // VkResult allocate_bind(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property);
        
        using _Base::create;
        using Memory::allocate;
    };

} // namespace cvk

#include "base_memorized.inl"