#ifndef __LOCK_FREE_THREAD_EXECUTOR_H__
#define __LOCK_FREE_THREAD_EXECUTOR_H__


#include "base_thread_executor.h"

namespace utils
{
    
class CPERF_API LockFreeThreadExecutor : public BaseThreadExecutor
{
public:
    using BaseThreadExecutor::BaseThreadExecutor;
    virtual ~LockFreeThreadExecutor() = default;

    void notify();
    
protected:
    void _wait_pause();
    void _wait_task_push();
};

} // namespace utils


#endif