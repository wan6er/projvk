#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "perf_basic.h"

#include "sync_tasks_state.h"
#include "thread_executor.h"

#include <vector>

namespace cperf
{

class CPERF_API ThreadPool
{
    using _ExecutorsType = std::vector<ThreadExecutor>;
    using _TaskStateType = std::shared_ptr<SyncTasksState>;

public:
    using TaskType = SyncTasksState::TaskType;

    ThreadPool(size_t size);
    ~ThreadPool();

    void push(TaskType task);
    void start();
    void pause();
    void stop();

    void wait_done();

private:
    void _init_executors(size_t size);

    _ExecutorsType _execs;
    _TaskStateType _tasks;

};

}

#endif