#pragma once

#include "cvk/vk_header.h"

namespace cvk
{
    
    class CVK_API BaseSync
    {
    public:
        virtual VkResult create(VkDevice device) = 0;
        virtual VkResult wait(uint32_t timeout);

    };

} // namespace cvk

#include "base_sync.inl"