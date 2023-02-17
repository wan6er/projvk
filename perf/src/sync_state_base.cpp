#include "sync_tasks_state.h"

namespace cperf
{

SyncTasksState::~SyncTasksState()
{
}

void SyncTasksState::push(TaskType task)
{
    queue.push_back(task);
    // _cv.notify_one();
    signal_task();
}

void SyncTasksState::start()
{
    state = ThreadState::RUNNING;
    _cv.notify_all();
}

void SyncTasksState::pause()
{
    state = ThreadState::PAUSE;
}

void SyncTasksState::stop()
{
    state = ThreadState::STOPPED;
    _cv.notify_all();
    _push_cv.notify_all();
}

void SyncTasksState::wait_finish()
{
    state = ThreadState::WAIT_FINISH;
    _cv.notify_all();
    _push_cv.notify_all();
}

void SyncTasksState::wait_task()
{
    std::unique_lock<std::mutex> locker(_push_mtx);
    _push_cv.wait(locker, [this]() -> bool { return queue.size() > 0 || state != ThreadState::RUNNING; });
}

void SyncTasksState::signal_task()
{
    _push_cv.notify_all();
}

void SyncTasksState::wait_finish_done()
{
    if (state == ThreadState::WAIT_FINISH && queue.size() == 0) {
        stop();
    }
}

void SyncTasksState::wait_all_tasks_done()
{
    std::unique_lock<std::mutex> locker(_tasks_mtx);
    _tasks_cv.wait(locker, [this]() -> bool { return queue.size() == 0; });
}

void SyncTasksState::signal_all_tasks_done()
{
    if (queue.size() == 0) {
        _tasks_cv.notify_one();
    }
}

void SyncTasksState::wait_pause()
{
    std::unique_lock<std::mutex> locker(_mtx);
    _cv.wait(locker, [this]() -> bool { return state != ThreadState::PAUSE; });
}

void SyncTasksState::notify_one()
{
    _cv.notify_one();
}


}