#pragma once

#include "cvk/sync/fence.h"
#include "cvk/sync/sync_set.h"

namespace cvk
{

    typedef SyncSet<Fence> FenceSet;
    
    template<>
    CVK_API VkResult FenceSet::wait(uint32_t timeout);
} // namespace cvk
