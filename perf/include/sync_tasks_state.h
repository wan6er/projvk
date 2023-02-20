#ifndef __SYNC_MEMORY_H__
#define __SYNC_MEMORY_H__

#include "perf_basic.h"
#include "sync_stack.h"

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

namespace utils
{

enum class ThreadState
{
    RUNNING,
    PAUSE,
    WAIT_FINISH,
    STOPPED,
};

class CPERF_API SyncTasksState
{
public:
    using TaskType = std::function<void()>;
    using ATStateType = std::atomic<ThreadState>;
    using ATTaskQueue = LockFreeStack<std::function<void()>>;

    virtual ~SyncTasksState();

    void push(TaskType task);
    auto pop() -> std::optional<TaskType>;
    size_t size() { return queue.size(); }

    void start();
    void pause();
    void stop();
    void wait_finish();

    void signal_pause();
    void wait_pause();
    void wait_task();
    void signal_task();

    void wait_finish_done();

    // auto get_queue() -> ATTaskQueue& { return queue; }
    auto get_state() -> ThreadState { return state; }

protected:
    ATTaskQueue queue;
    ATStateType state = ThreadState::RUNNING;

private:
    std::mutex _push_mtx;
    std::condition_variable _push_cv;

    std::mutex _pause_mtx;
    std::condition_variable _pause_cv;
};

} // namespace cperf

#endif