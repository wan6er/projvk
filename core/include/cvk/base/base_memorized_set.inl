#pragma once

namespace cvk
{
    
template<class _Base>
BaseMemorizedSet<_Base>::BaseMemorizedSet(VkDevice device) :
    _Base(device),
    Memory(device)
{
}

template<class _Base>
BaseMemorizedSet<_Base>::~BaseMemorizedSet()
{
}

// template<class _Base>
// VkResult BaseMemorizedSet<_Base>::allocate_bind(VkMemoryPropertyFlags properties, uint32_t size, uint32_t memory_type)
// {
//     VkResult result = VK_SUCCESS;
//     std::vector<_Base> CONST_REFERENCE _set = *this;
//     std::vector<VkDeviceSize> CONST_REFERENCE _offsets = *this;
//     CVK_API(_set.size() == _offsets.size());

//     result = Memory::allocate(properties, _Base::get_memory_requirement(), property);
//     if (result != VK_SUCCESS) {
//         goto __RESULT;
//     }

//     for (uint32_t i = 0; i < _set.size(); ++i) {
//         result = Memory::bind(_set[i], _offsets[i]);
//         if (result != VK_SUCCESS) {
//             goto __RESULT;
//         }
//     }
// __RESULT:
//     return result;
// }

} // namespace cvk

