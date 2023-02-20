#include "base_thread_executor.h"


namespace utils
{

BaseThreadExecutor::BaseThreadExecutor(TasksStateType state) :
    _state(state),
    _thr(&BaseThreadExecutor::_task_loop, this)
{
}

BaseThreadExecutor::BaseThreadExecutor(BaseThreadExecutor const& exec) :
    _state(exec._state),
    _thr(&BaseThreadExecutor::_task_loop, this)
{
}

BaseThreadExecutor::~BaseThreadExecutor()
{
}

void BaseThreadExecutor::join()
{
    if (_thr.joinable()) {
        _thr.join();
    }
}

BaseThreadExecutor& BaseThreadExecutor::operator=(BaseThreadExecutor const& exec)
{
    _state = exec._state;
    _thr = std::thread(&BaseThreadExecutor::_task_loop, this);
    return *this;
}

void BaseThreadExecutor::notify()
{
    _pause_cv.notify_one();
}

void BaseThreadExecutor::_wait_pause()
{
    // std::unique_lock<std::mutex> locker(_pause_mtx);
    // _pause_cv.wait(locker, [this]() -> bool { return _state->get_state() != ThreadState::PAUSE; });
    _state->wait_pause();
}

void BaseThreadExecutor::_wait_task_push()
{
    _wait([this]() { return _state->size() > 0 || _state->get_state() != ThreadState::RUNNING; });
}


void BaseThreadExecutor::_wait(std::function<bool()> predicate)
{
    while (!predicate()) {
        std::this_thread::yield();
    }
}

void BaseThreadExecutor::wait_task()
{
    _state->signal_pause();
    _wait([this]() { return _has_waited.load(); });
}

void BaseThreadExecutor::_task_loop()
{
    while (_state->get_state() != ThreadState::STOPPED) {

        auto local_task = _state->pop();
        if (local_task) {
            (*local_task)();
        } else {
            _has_waited.store(true);
            _wait_task_push();
            _has_waited.store(false);
        }

        _state->wait_finish_done();
        _wait_pause();
    }
}

}; // namespace cperf
