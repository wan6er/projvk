#pragma once

namespace cvk
{

template<class _BaseType>
constexpr SyncSet<_BaseType>::SyncSet(VkDevice device, uint32_t size, bool signaled) :
    _device(device)
{
    std::vector<_BaseType>& set = this->get_attachments();
    for (uint32_t i = 0; i < size; ++i) {
        set.emplace_back(_device, signaled);
    }
}

template<class _BaseType>
constexpr SyncSet<_BaseType>::SyncSet(VkDevice device) :
    _device(device)
{
}


template<class _BaseType>
VkResult SyncSet<_BaseType>::create()
{
    _is_created = true;
    VkResult result = VK_SUCCESS;
    std::vector<_BaseType>& set = this->get_attachments();
    for (auto& item : set) {
        result = item.create();
        if (result != VK_SUCCESS) {
            break;
        }
    }
    return result;
}

// template<class >
// VkResult SyncSet<>::wait(uint32_t timeout);
template<class _BaseType>
VkResult SyncSet<_BaseType>::wait(uint32_t timeout)
{
    return VK_ERROR_UNKNOWN;
}

template<class _BaseType>
constexpr auto SyncSet<_BaseType>::operator[](size_t i) -> _BaseType&
{
    std::vector<_BaseType>& set = this->get_attachments();
    return set[i];
}

template<class _BaseType>
constexpr auto SyncSet<_BaseType>::operator=(std::initializer_list<bool> CONST_REFERENCE signaled)
{
    CVK_ASSERT(_is_created == false);
    this->clear_all();
    std::vector<_BaseType>& set = this->get_attachments();
    for (auto iter = signaled.begin(); iter != signaled.end(); iter++) {
        set.emplace_back(_device, iter);
    }
}

};