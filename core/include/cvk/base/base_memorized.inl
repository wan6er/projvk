#pragma once

namespace cvk
{
    
template<class _Base>
BaseMemorized<_Base>::BaseMemorized(VkDevice device) :
    _Base(device),
    Memory(device)
{
}
template<class _Base>
BaseMemorized<_Base>::~BaseMemorized()
{
}

template<class _Base>
template<class...__Args>
VkResult BaseMemorized<_Base>::create(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryPropertyFlags property, __Args&&...args)
{
    VkResult result = VK_SUCCESS;
    result = _Base::create(std::forward<__Args>(args)...);
    if (result != VK_SUCCESS) {
        goto __RESULT;
    }

    result = Memory::allocate(properties, _Base::get_memory_requirement(), property);
    if (result != VK_SUCCESS) {
        _Base::release();
        goto __RESULT;
    }

    result = Memory::bind(*this);
    if (result != VK_SUCCESS) {
        Memory::release();
        _Base::release();
        goto __RESULT;
    }
__RESULT:
    return result;
}

} // namespace cvk

