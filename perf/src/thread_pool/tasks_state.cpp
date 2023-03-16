#include "thread_pool/tasks_state.h"

namespace utils
{

TasksState::~TasksState()
{
}

void TasksState::push(TaskType task)
{
    queue.push(task);
}

auto TasksState::pop() -> SharedTaskPtr
{
    return queue.pop();
}

void TasksState::start()
{
    state = ThreadState::RUNNING;
}

void TasksState::pause()
{
    state = ThreadState::PAUSE;
}

void TasksState::stop()
{
    state = ThreadState::STOPPED;
}

void TasksState::wait_finish()
{
    state = ThreadState::WAIT_FINISH;
}

// void TasksState::signal_pause()
// {
//     _pause_cv.notify_one();
// }

// void TasksState::wait_pause()
// {
//     std::unique_lock<std::mutex> locker(_pause_mtx);
//     _pause_cv.wait(locker, [this]() -> bool { return state.load() != ThreadState::PAUSE; });
// }

// void TasksState::wait_task()
// {
//     std::unique_lock<std::mutex> locker(_push_mtx);
//     _push_cv.wait(locker, [this]() -> bool { return queue.size() > 0 || state != ThreadState::RUNNING; });
// }

// void TasksState::signal_task()
// {
//     _push_cv.notify_one();
// }

void TasksState::wait_finish_done()
{
    if (state == ThreadState::WAIT_FINISH && queue.size() == 0) {
        stop();
    }
}

}