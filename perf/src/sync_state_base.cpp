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
    state = ThreadState::RUNNING;
    _push_cv.notify_all();
}

void SyncTasksState::pause()
{
    state = ThreadState::PAUSE;
    _push_cv.notify_all();
}

void SyncTasksState::stop()
{
    state = ThreadState::STOPPED;
    _push_cv.notify_all();
}

void SyncTasksState::wait_finish()
{
    state = ThreadState::WAIT_FINISH;
    _push_cv.notify_all();
}

void SyncTasksState::signal_pause()
{
    _pause_cv.notify_all();
}
void SyncTasksState::wait_pause()
{
    std::unique_lock<std::mutex> locker(_pause_mtx);
    _pause_cv.wait(locker, [this]() -> bool { return state != ThreadState::PAUSE; });
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