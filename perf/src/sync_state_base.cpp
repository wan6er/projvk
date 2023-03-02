#include "sync_tasks_state.h"

namespace utils
{

SyncTasksState::~SyncTasksState()
{
}

void SyncTasksState::push(TaskType task)
{
    queue.push(task);
    // _cv.notify_one();
    signal_task();
}

auto SyncTasksState::pop() -> std::optional<TaskType>
{
    return queue.pop();
}

void SyncTasksState::start()
{
    {
        std::lock_guard locker(_pause_mtx);
        state = ThreadState::RUNNING;
    }
    _pause_cv.notify_all();
    _push_cv.notify_all();
}

void SyncTasksState::pause()
{
    {
        std::lock_guard locker(_pause_mtx);
        state = ThreadState::PAUSE;
    }
    _push_cv.notify_all();
}

void SyncTasksState::stop()
{
    {
        std::lock_guard locker(_pause_mtx);
        state = ThreadState::STOPPED;
    }
    _pause_cv.notify_all();
    _push_cv.notify_all();
}

void SyncTasksState::wait_finish()
{
    {
        std::lock_guard locker(_pause_mtx);
        state = ThreadState::WAIT_FINISH;
    }
    _pause_cv.notify_all();
    _push_cv.notify_all();
}

void SyncTasksState::signal_pause()
{
    _pause_cv.notify_one();
}

void SyncTasksState::wait_pause()
{
    std::unique_lock<std::mutex> locker(_pause_mtx);
    _pause_cv.wait(locker, [this]() -> bool { return state.load() != ThreadState::PAUSE; });
}

void SyncTasksState::wait_task()
{
    std::unique_lock<std::mutex> locker(_push_mtx);
    _push_cv.wait(locker, [this]() -> bool { return queue.size() > 0 || state != ThreadState::RUNNING; });
}

void SyncTasksState::signal_task()
{
    _push_cv.notify_one();
}

void SyncTasksState::wait_finish_done()
{
    if (state == ThreadState::WAIT_FINISH && queue.size() == 0) {
        stop();
    }
}

}