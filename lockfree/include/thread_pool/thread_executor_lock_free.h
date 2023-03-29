#ifndef __LOCK_FREE_THREAD_EXECUTOR_H__
#define __LOCK_FREE_THREAD_EXECUTOR_H__


#include "thread_executor_base.h"

namespace utils
{
    
class CLF_API LockFreeThreadExecutor : public BaseLockFreeThreadExecutor
{
public:
    using BaseLockFreeThreadExecutor::BaseLockFreeThreadExecutor;
    LockFreeThreadExecutor(LockFreeThreadExecutor const& exec);
    virtual ~LockFreeThreadExecutor() = default;

    auto operator=(LockFreeThreadExecutor const& exec) -> LockFreeThreadExecutor&;

};

} // namespace utils


#endif