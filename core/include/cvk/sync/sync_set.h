#pragma once

#include "semaphore.h"
#include "fence.h"

#include "cvk/vk_header.h"
#include "utils/base_multi_attach.h"

namespace cvk
{
    
    template<class _BaseType>
    class SyncSet : 
        public utils::BaseMultipleAttaches<std::enable_if_t<std::is_base_of_v<BaseSync, _BaseType>, _BaseType>>
    {
    public:
        constexpr SyncSet() = default;
        constexpr SyncSet(uint32_t size, bool signaled = false);
        constexpr SyncSet(std::initializer_list<bool> CONST_REFERENCE signaled);

        VkResult create(VkDevice device);
        VkResult wait(uint32_t timeout = UINT32_MAX);

        constexpr auto operator[](uint32_t i) -> _BaseType&;

    private:
        VkDevice _device;
    };

    typedef SyncSet<Semaphore> SemaphoreSet;
    typedef SyncSet<Fence> FenceSet;
    
    template<>
    CVK_API VkResult FenceSet::wait(uint32_t timeout);

}; // namespace cvk

#include "sync_set.inl"