#ifndef __TASKS_STATE_H__
#define __TASKS_STATE_H__

#include "perf_basic.h"
#include "lock_free/sync_stack.h"

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

class CPERF_API TasksState
{
public:
    using TaskType = std::function<void()>;
    using ATStateType = std::atomic<ThreadState>;
    using ATTaskQueue = LockFreeStack<std::function<void()>>;
    using SharedTaskPtr = ATTaskQueue::SharedNodePtr;

    virtual ~TasksState();

    void push(TaskType task);
    auto pop() -> SharedTaskPtr;
    size_t size() { return queue.size(); }

    virtual void start();
    virtual void pause();
    virtual void stop();
    virtual void wait_finish();

    virtual void wait_finish_done();

    // void signal_pause();
    // void wait_pause();
    // void wait_task();
    // void signal_task();

    // auto get_queue() -> ATTaskQueue& { return queue; }
    auto get_state() -> ThreadState { return state; }

protected:
    // std::mutex _pause_mtx;
    // std::condition_variable _pause_cv;

    ATStateType state = ThreadState::RUNNING;
    ATTaskQueue queue;

private:
    // std::mutex _push_mtx;
    // std::condition_variable _push_cv;

};

} // namespace cperf

#endif