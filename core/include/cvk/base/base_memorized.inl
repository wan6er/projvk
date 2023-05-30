#pragma once

namespace cvk
{
    
template<class _Base, class _Memory>
BaseMemorized<_Base, _Memory>::BaseMemorized(VkDevice device) :
    _Base(device),
    _Memory(device)
{
}

template<class _Base, class _Memory>
BaseMemorized<_Base, _Memory>::BaseMemorized(VkDevice device, VkDeviceMemory memory) :
    _Base(device),
    _Memory(device)
{
}

// template<class _Base, class _Memory>
// template<class...__Args>
// VkResult BaseMemorized<_Base, _Memory>::create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property, __Args&&...args)
// {
//     VkResult result = VK_SUCCESS;
    
//     if constexpr (sizeof...(args) == 0) {
//         result = _Base::create();
//     } else {
//         result = _Base::create(std::forward<__Args>(args)...);
//     }
//     if (result != VK_SUCCESS) {
//         goto __RESULT;
//     }

//     result = Memory::allocate(properties, _Base::get_memory_requirement(), property);
//     if (result != VK_SUCCESS) {
//         _Base::release();
//         goto __RESULT;
//     }

//     result = Memory::bind(*this);
//     if (result != VK_SUCCESS) {
//         _Base::release();
//         Memory::deallocate();
//         goto __RESULT;
//     }
// __RESULT:
//     return result;
// }


template<class _Base, class _Memory>
template<class...__Args>
VkResult BaseMemorized<_Base, _Memory>::create_obj(__Args&&...args)
{

    if constexpr (sizeof...(args) == 0) {
        return _Base::create();
    } else {
        return _Base::create(std::forward<__Args>(args)...);
    }
}

template<class _Base, class _Memory>
template<class...__Args>
VkResult BaseMemorized<_Base, _Memory>::create_mem(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property)
{
    auto requirements = _Base::get_memory_requirement();
    return _Memory::allocate(properties, requirements, property);
}

template<class _Base, class _Memory>
VkResult BaseMemorized<_Base, _Memory>::bind()
{
    return _Memory::bind(*this);
}

} // namespace cvk


namespace cvk
{

template<class _Base, VkMemoryPropertyFlags _Type>
template<class...__Args>
VkResult BaseTypeMemorized<_Base, _Type>::create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, __Args&&...args)
{
    VkResult result = VK_SUCCESS;
    if ((result = this->create_obj(std::forward<__Args>(args)...))) {
        return result;
    }

    if ((result = this->create_mem(properties, _Type))) {
        return result;
    }

    return this->bind();
}

}
