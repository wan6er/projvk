#pragma once

namespace cvk
{

template<class _BaseType>
constexpr SyncSet<_BaseType>::SyncSet(uint32_t size, bool signaled)
{
    std::vector<_BaseType>& set = this->get_attachments();
    for (uint32_t i = 0; i < size; ++i) {
        set.emplace_back(signaled);
    }
}

template<class _BaseType>
constexpr SyncSet<_BaseType>::SyncSet(std::initializer_list<bool> CONST_REFERENCE signaled)
{
    std::vector<_BaseType>& set = this->get_attachments();
    set.insert(set.end(), signaled.begin(), signaled.end());
}


template<class _BaseType>
VkResult SyncSet<_BaseType>::create(VkDevice device)
{
    VkResult result = VK_SUCCESS;
    _device = device;
    std::vector<_BaseType>& set = this->get_attachments();
    for (auto& item : set) {
        result = item.create(_device);
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
constexpr auto SyncSet<_BaseType>::operator[](uint32_t i) -> _BaseType&
{
    std::vector<_BaseType>& set = this->get_attachments();
    return set[i];
}

};