#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "perf_basic.h"

#include "sync_tasks_state.h"
#include "base_thread_executor.h"
#include "lock_thread_executor.h"
#include "lock_free_thread_executor.h"

#include <vector>

namespace utils
{

template<typename _Exec>
class BaseThreadPool
{
    using _ExecutorsType = std::vector<_Exec>;
    using _TaskStateType = std::shared_ptr<SyncTasksState>;

public:
    using TaskType = SyncTasksState::TaskType;

    BaseThreadPool(size_t size);
    ~BaseThreadPool();

    void push(TaskType task);
    void start();
    void pause();
    void stop();

    void wait_done();

private:
    void _notify_all();
    void _init_executors(size_t size);

    _ExecutorsType _execs;
    _TaskStateType _tasks;

};

using ThreadPool = BaseThreadPool<BaseThreadExecutor>;
using LockThreadPool = BaseThreadPool<LockThreadExecutor>;
using LockFreeThreadPool = BaseThreadPool<LockFreeThreadExecutor>;

}

#endif