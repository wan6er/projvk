#ifndef __THREAD_EXECUTOR_H__
#define __THREAD_EXECUTOR_H__

#include "perf_basic.h"
#include "sync_tasks_state.h"

#include <thread>
#include <atomic>

namespace utils
{

class CPERF_API BaseThreadExecutor
{
public:
    using TaskType = SyncTasksState::TaskType;
    using TasksStateType = std::shared_ptr<SyncTasksState>;

    BaseThreadExecutor(TasksStateType state);
    BaseThreadExecutor(BaseThreadExecutor const& exec);
    virtual ~BaseThreadExecutor();
    
    BaseThreadExecutor& operator=(BaseThreadExecutor const& exec);

    void join();
    virtual void notify();
    // bool running() const { return !_is_pause; }
    void wait_task();

protected:
    void _wait(std::function<bool()> compare);

    virtual void _wait_pause();
    virtual void _wait_task_push();
    
    TasksStateType _state;

private:
    void _task_loop();
    
    // void _notify_taskover();
    
    std::thread _thr;

    std::mutex _pause_mtx;
    std::condition_variable _pause_cv;
    
    std::atomic<bool> _has_waited = false;
};

} // namespace cperf


#endif