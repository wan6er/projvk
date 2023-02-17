#ifndef __THREAD_EXECUTOR_H__
#define __THREAD_EXECUTOR_H__

#include "perf_basic.h"
#include "sync_tasks_state.h"

#include <thread>
#include <atomic>

namespace utils
{

class CPERF_API ThreadExecutor
{
public:
    using TaskType = SyncTasksState::TaskType;
    using TasksStateType = std::shared_ptr<SyncTasksState>;

    ThreadExecutor(TasksStateType state);
    ThreadExecutor(ThreadExecutor const& exec);
    virtual ~ThreadExecutor();
    
    ThreadExecutor& operator=(ThreadExecutor const& exec);

    void join();
    void notify();
    // bool running() const { return !_is_pause; }
    void wait_task();

private:
    void _task_loop();
    void _wait_pause();
    void _notify_taskover();
    
    TasksStateType _state;
    std::thread _thr;

    std::mutex _pause_mtx;
    std::condition_variable _pause_cv;
    
    std::mutex _tasks_mtx;
    std::condition_variable _tasks_cv;

    std::atomic<bool> _has_waited = false;
};

} // namespace cperf


#endif