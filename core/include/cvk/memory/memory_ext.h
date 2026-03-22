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
        constexpr MemoryExt(VkDevice device, VkDeviceMemory memory);

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
        using BaseMemoryExt = MemoryExt<_Memory>;
        using BaseMemory = _Memory;

    public:
        constexpr BaseTypeMemoryExt(VkDevice device, void CONST_PTR next = nullptr);
        constexpr BaseTypeMemoryExt(VkDevice device, VkDeviceMemory memory);

    };

} // namespace cvk

namespace cvk
{

    template<class _Memory>
    constexpr MemoryExt<_Memory>::MemoryExt(VkDevice device, void CONST_PTR next) :
        BaseMemory(device, &_flag_info)
    {
        _flag_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
        _flag_info.pNext = next;
    }
    
    template<class _Memory>
    constexpr MemoryExt<_Memory>::MemoryExt(VkDevice device, VkDeviceMemory memory) :
        BaseMemory(device, memory)
    {
    }

    template<class _Memory>
    void MemoryExt<_Memory>::set_flags(VkMemoryAllocateFlags flags)
    {
        _flag_info.flags = flags;
    }

    template<class _Memory>
    void MemoryExt<_Memory>::set_mask(uint32_t mask)
    {
        _flag_info.deviceMask = mask;
    }

} // namespace cvk

namespace cvk
{

    template<class _Memory, VkMemoryPropertyFlags _Flag, uint32_t _Mask>
    constexpr BaseTypeMemoryExt<_Memory, _Flag, _Mask>::BaseTypeMemoryExt(VkDevice device, void CONST_PTR next) :
        MemoryExt<_Memory>(device, next)
    {
        BaseMemoryExt::set_flags(_Flag);
        BaseMemoryExt::set_mask(_Mask);
    }

    template<class _Memory, VkMemoryPropertyFlags _Flag, uint32_t _Mask>
    constexpr BaseTypeMemoryExt<_Memory, _Flag, _Mask>::BaseTypeMemoryExt(VkDevice device, VkDeviceMemory memory) :
        MemoryExt<_Memory>(device, memory)
    {
        BaseMemoryExt::set_flags(_Flag);
        BaseMemoryExt::set_mask(_Mask);
    }

} // namespace cvk

#include "memory_ext.inl"