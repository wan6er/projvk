#pragma once

#include "cvk/vk_header.h"
#include "cvk/memory.h"
#include "utils/base_multi_attach.h"

#include <type_traits>

namespace cvk
{
    template<class _Base>
    class BaseMemorizedSet : 
        public utils::BaseMultipleAttaches<_Base, VkDeviceSize>,
        public Memory
    {
    public:
        virtual ~BaseMemorizedSet();

    protected:
        BaseMemorizedSet(VkDevice device);
        VkResult allocate_bind();
        
    private:
        using Memory::allocate;

    };

} // namespace cvk

#include "base_memorized_set.inl"