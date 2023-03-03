#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "perf_basic.h"

#include "tasks_state.h"
#include "thread_executor.h"
#include "thread_executor_lock_free.h"

#include <vector>

namespace utils
{

template<typename _Exec, typename _Tasks>
class BaseThreadPool
{
    using _ExecutorsType = std::vector<_Exec>;
    using _TaskStateType = std::shared_ptr<_Tasks>;

public:
    using TaskType = typename _Tasks::TaskType;

    BaseThreadPool(size_t size);
    ~BaseThreadPool();

    void push(TaskType task);
    void start();
    void pause();
    void stop();

    void wait_done();

private:
    // void _notify_all();
    void _init_executors(size_t size);

    _ExecutorsType _execs;
    _TaskStateType _tasks;

};

using ThreadPool = BaseThreadPool<ThreadExecutor, ThreadExecutor::TasksStateType>;
using LockFreeThreadPool = BaseThreadPool<LockFreeThreadExecutor, LockFreeThreadExecutor::TasksStateType>;

}

#endif