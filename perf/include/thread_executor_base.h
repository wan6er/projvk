#ifndef __BASE_THREAD_EXECUTOR_H__
#define __BASE_THREAD_EXECUTOR_H__

#include "perf_basic.h"
#include "tasks_state.h"
#include "tasks_state_pause_lock.h"

#include <thread>
#include <atomic>

namespace utils
{

template<typename _TaskState>
class CPERF_API BaseThreadExecutorImpl
{
public:
    using TaskType = typename _TaskState::TaskType;
    using TasksStateType = _TaskState;
    using TasksStatePtrType = std::shared_ptr<TasksStateType>;

    BaseThreadExecutorImpl(TasksStatePtrType state);
    BaseThreadExecutorImpl(BaseThreadExecutorImpl const& exec);
    virtual ~BaseThreadExecutorImpl();
    
    auto operator=(BaseThreadExecutorImpl const& exec) -> BaseThreadExecutorImpl&;

    void join();
    virtual void wait_task();

protected:
    void _wait(std::function<bool()> compare);

    virtual void _wait_pause();
    virtual void _wait_task_push();
    
    TasksStatePtrType _state;

private:
    void _task_loop();
    
    // void _notify_taskover();
    
    std::atomic<int> _has_waited = 0;
    std::thread _thr;

    // std::mutex _pause_mtx;
    // std::condition_variable _pause_cv;
    // volatile int _has_waited = false;
};

using BaseLockFreeThreadExecutor = BaseThreadExecutorImpl<TasksState>;
using BaseThreadExecutor = BaseThreadExecutorImpl<TasksStatePauseLock>;

} // namespace cperf


#endif