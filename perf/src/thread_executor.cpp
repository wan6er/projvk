#include "thread_executor.h"

#include <iostream>

namespace cperf
{

ThreadExecutor::ThreadExecutor(TasksStateType state) :
    _state(state),
    _thr(&ThreadExecutor::task_loop, this)
{
}

ThreadExecutor::ThreadExecutor(ThreadExecutor const& exec) :
    _state(exec._state),
    _thr(&ThreadExecutor::task_loop, this)
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
    _thr = std::thread(&ThreadExecutor::task_loop, this);
    return *this;
}

void ThreadExecutor::task_loop()
{
    
    while (_state->get_state() != ThreadState::STOPPED) {
        _state->wait_task();
        // std::cout << std::this_thread::get_id() << " => wait_task\n";

        auto local_task = _state->get_queue().pop_front();
        if (local_task) {
            (*local_task)();
            // std::cout << std::this_thread::get_id() << " => proc task\n";
        }

        _state->signal_all_tasks_done();
        _state->wait_finish_done();
        _state->wait_pause();
        // std::cout << std::this_thread::get_id() << " => wait_pause\n";
    }
}

}; // namespace cperf
