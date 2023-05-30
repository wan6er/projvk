
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
        set_flags(_Flag);
        set_mask(_Mask);
    }

    template<class _Memory, VkMemoryPropertyFlags _Flag, uint32_t _Mask>
    constexpr BaseTypeMemoryExt<_Memory, _Flag, _Mask>::BaseTypeMemoryExt(VkDevice device, VkDeviceMemory memory) :
        MemoryExt<_Memory>(device, memory)
    {
        set_flags(_Flag);
        set_mask(_Mask);
    }

} // namespace cvk
