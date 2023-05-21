#pragma once

#include "utils/base_obj.h"
#include "cvk/vk_header.h"
#include "cvk/memory/memory.h"

namespace cvk
{

    template<class _Memory>
    class MemoryExt : public _Memory
    {
        using BaseMemory = _Memory;

    public:
        constexpr MemoryExt(VkDevice device, void CONST_PTR next = nullptr);

        void set_flags(VkMemoryAllocateFlags flags);
        void set_mask(uint32_t mask);

    protected:
        inline VkMemoryAllocateFlagsInfo& flag_info() { return _flag_info; }

    private:
        VkMemoryAllocateFlagsInfo _flag_info {};

    };

    template<class _Memory, VkMemoryPropertyFlags _Flag, uint32_t _Mask>
    class BaseTypeMemoryExt : public MemoryExt<_Memory>
    {
        using BaseMemoryExt = _Memory;

    public:
        constexpr BaseTypeMemoryExt(VkDevice device, void CONST_PTR next = nullptr);

    };

} // namespace cvk

#include "memory_ext.inl"