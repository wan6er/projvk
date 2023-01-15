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
        // constexpr SyncSet() = default;
        constexpr SyncSet(VkDevice device, uint32_t size, bool signaled = false);
        constexpr SyncSet(VkDevice device);

        VkResult create();
        VkResult wait(uint32_t timeout = UINT32_MAX);

        constexpr auto operator[](size_t i) -> _BaseType&;
        constexpr auto operator=(std::initializer_list<bool> CONST_REFERENCE signaled);

    private:
        VkDevice _device;
        bool _is_created = false;

    };

    

}; // namespace cvk

#include "sync_set.inl"