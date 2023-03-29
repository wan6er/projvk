#ifndef __THREAD_EXECUTOR_H__
#define __THREAD_EXECUTOR_H__

#include "thread_executor_base.h"

#include <mutex>
#include <condition_variable>

namespace utils
{
    
class CLF_API ThreadExecutor : public BaseThreadExecutor
{
public:
    using BaseThreadExecutor::BaseThreadExecutor;
    ThreadExecutor(ThreadExecutor const& exec);
    virtual ~ThreadExecutor() = default;

    auto operator=(ThreadExecutor const& exec) -> ThreadExecutor&;
    
protected:
    void _wait_pause();
    // void _wait_task_push();
    
    std::mutex _pause_mtx;
    std::condition_variable _pause_cv;
};

} // namespace utils


#endif