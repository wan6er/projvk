
namespace utils
{
template<typename _TaskState>
BaseThreadExecutorImpl<_TaskState>::BaseThreadExecutorImpl(TasksStatePtrType state) :
    _state(state),
    _thr(&BaseThreadExecutorImpl<_TaskState>::_task_loop, this)
{
}

template<typename _TaskState>
BaseThreadExecutorImpl<_TaskState>::BaseThreadExecutorImpl(BaseThreadExecutorImpl const& exec) :
    _state(exec._state),
    _thr(&BaseThreadExecutorImpl::_task_loop, this)
{
}

template<typename _TaskState>
BaseThreadExecutorImpl<_TaskState>::~BaseThreadExecutorImpl()
{
}

template<typename _TaskState>
void BaseThreadExecutorImpl<_TaskState>::join()
{
    if (_thr.joinable()) {
        _thr.join();
    }
}

template<typename _TaskState>
auto BaseThreadExecutorImpl<_TaskState>::operator=(BaseThreadExecutorImpl const& exec) -> BaseThreadExecutorImpl&
{
    _state = exec._state;
    _thr = std::thread(&BaseThreadExecutorImpl::_task_loop, this);
    return *this;
}

// template<typename _TaskState>
// void BaseThreadExecutorImpl<_TaskState>::notify()
// {
//     _pause_cv.notify_one();
// }

// template<typename _TaskState>
// void BaseThreadExecutorImpl<_TaskState>::_wait_pause()
// {
//     _state->wait_pause();
// }

template<typename _TaskState>
void BaseThreadExecutorImpl<_TaskState>::_wait_pause()
{
    _wait([this]() { return _state->get_state() != ThreadState::PAUSE; });
}

template<typename _TaskState>
void BaseThreadExecutorImpl<_TaskState>::_wait_task_push()
{
    _wait([&]() { return _state->size() > 0 || _state->get_state() != ThreadState::RUNNING; });
}


template<typename _TaskState>
void BaseThreadExecutorImpl<_TaskState>::_wait(std::function<bool()> predicate)
{
    while (!predicate()) {
        std::this_thread::yield();
    }
}

template<typename _TaskState>
void BaseThreadExecutorImpl<_TaskState>::wait_task()
{
    _wait([this]() -> bool { return _has_waited.load() == 1; });
}

template<typename _TaskState>
void BaseThreadExecutorImpl<_TaskState>::_task_loop()
{
    while (_state->get_state() != ThreadState::STOPPED) {

        auto local_task = _state->pop();
        if (local_task) {
            (*local_task)();
        } else {
            _has_waited.store(1);
            _wait_task_push();
            _has_waited.store(0);
        }

        _state->wait_finish_done();
        _wait_pause();
    }
}

}; // namespace cperf
