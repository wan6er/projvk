#include "thread_executor.h"

#include <iostream>

namespace utils
{

ThreadExecutor::ThreadExecutor(TasksStateType state) :
    _state(state),
    _thr(&ThreadExecutor::_task_loop, this)
{
}

ThreadExecutor::ThreadExecutor(ThreadExecutor const& exec) :
    _state(exec._state),
    _thr(&ThreadExecutor::_task_loop, this)
{
}

ThreadExecutor::~ThreadExecutor()
{
}

void ThreadExecutor::join()
{
    if (_thr.joinable()) {
        _thr.join();
    }
}

ThreadExecutor& ThreadExecutor::operator=(ThreadExecutor const& exec)
{
    _state = exec._state;
    _thr = std::thread(&ThreadExecutor::_task_loop, this);
    return *this;
}

void ThreadExecutor::notify()
{
    _pause_cv.notify_one();
}

void ThreadExecutor::_wait_pause()
{
    std::unique_lock<std::mutex> locker(_pause_mtx);
    _pause_cv.wait(locker, [this]() -> bool { return _state->get_state() != ThreadState::PAUSE; });
}

void ThreadExecutor::wait_task()
{
    while (!_has_waited) {
        std::this_thread::yield();
    }
}

void ThreadExecutor::_task_loop()
{
    while (_state->get_state() != ThreadState::STOPPED) {

        auto local_task = _state->get_queue().pop_front();
        if (local_task) {
            (*local_task)();
        } else {
            _has_waited.store(true);
            _state->wait_task();
            _has_waited.store(false);
        }

        _state->wait_finish_done();
        _wait_pause();
    }
}

}; // namespace cperf
