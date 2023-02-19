#ifndef __LOCK_THREAD_EXECUTOR_H__
#define __LOCK_THREAD_EXECUTOR_H__


#include "base_thread_executor.h"

namespace utils
{
    
class CPERF_API LockThreadExecutor : public BaseThreadExecutor
{
public:
    using BaseThreadExecutor::BaseThreadExecutor;
    virtual ~LockThreadExecutor() = default;

protected:
    void _wait_task_push();
};

} // namespace utils


#endif