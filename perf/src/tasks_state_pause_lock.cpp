#include "tasks_state_pause_lock.h"

namespace utils
{
    
    
void TasksStatePauseLock::start()
{
    {
        std::lock_guard locker(_pause_mtx);
        _BaseType::start();
    }
    _pause_cv.notify_all();
}

void TasksStatePauseLock::pause()
{
    {
        std::lock_guard locker(_pause_mtx);
        _BaseType::pause();
    }
    _pause_cv.notify_all();
}

void TasksStatePauseLock::stop()
{
    {
        std::lock_guard locker(_pause_mtx);
        _BaseType::stop();
    }
    _pause_cv.notify_all();
}

void TasksStatePauseLock::wait_finish()
{
    {
        std::lock_guard locker(_pause_mtx);
        _BaseType::wait_finish();
    }
    _pause_cv.notify_all();
}


void TasksStatePauseLock::signal_pause()
{
    _pause_cv.notify_one();
}

void TasksStatePauseLock::wait_pause()
{
    std::unique_lock<std::mutex> locker(_pause_mtx);
    _pause_cv.wait(locker, [this]() -> bool { return this->get_state() != ThreadState::PAUSE; });
}

} // namespace utils
