#ifndef __THREAD_EXECUTOR_H__
#define __THREAD_EXECUTOR_H__

#include "perf_basic.h"
#include "sync_tasks_state.h"

#include <thread>

namespace cperf
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

private:
    void task_loop();
    
    TasksStateType _state;
    std::thread _thr;

};

} // namespace cperf


#endif