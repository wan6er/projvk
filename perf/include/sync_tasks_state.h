#ifndef __SYNC_MEMORY_H__
#define __SYNC_MEMORY_H__

#include "perf_basic.h"
#include "sync_queue.h"

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
    using ATTaskQueue = SyncQueue<std::function<void()>>;

    virtual ~SyncTasksState();

    void push(TaskType task);
    
    void start();
    void pause();
    void stop();
    void wait_finish();

    void wait_task();
    void signal_task();
    // void wait_all_tasks_done();
    // void signal_all_tasks_done();
    void wait_finish_done();
    // void wait_pause();
    // void notify_one();

    auto get_queue() -> ATTaskQueue& { return queue; }
    auto get_state() -> ThreadState { return state; }

protected:
    ATTaskQueue queue;
    ATStateType state = ThreadState::RUNNING;

private:
    std::mutex _push_mtx;
    std::condition_variable _push_cv;

};

} // namespace cperf

#endif