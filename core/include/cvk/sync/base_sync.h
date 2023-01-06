#pragma once

#include "cvk/vk_header.h"

namespace cvk
{
    
    class CVK_API BaseSync
    {
    public:
        virtual VkResult create() = 0;
        virtual void set_create_signal(bool signaled) = 0;
        virtual VkResult wait(uint32_t timeout);

    };

} // namespace cvk

#include "base_sync.inl"