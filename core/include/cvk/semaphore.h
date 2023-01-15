#pragma once

#include "cvk/sync/semaphore.h"
#include "cvk/sync/sync_set.h"

namespace cvk
{
    
    typedef SyncSet<Semaphore> SemaphoreSet;

} // namespace cvk
