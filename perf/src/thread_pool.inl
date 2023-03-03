#include "thread_pool.h"

#include <iostream>

namespace utils
{
    
template<typename _Exec, typename _Tasks>
BaseThreadPool<_Exec, _Tasks>::BaseThreadPool(size_t size) :
    _tasks(std::make_shared<_Tasks>())
{
    _init_executors(size);
}

template<typename _Exec, typename _Tasks>
BaseThreadPool<_Exec, _Tasks>::~BaseThreadPool()
{
    _tasks->stop();
    for (auto& t : _execs) {
        t.join();
    }
}

template<typename _Exec, typename _Tasks>
void BaseThreadPool<_Exec, _Tasks>::push(TaskType task)
{
    _tasks->push(task);
}

template<typename _Exec, typename _Tasks>
void BaseThreadPool<_Exec, _Tasks>::start()
{
    _tasks->start();
}

template<typename _Exec, typename _Tasks>
void BaseThreadPool<_Exec, _Tasks>::pause()
{
    _tasks->pause();
}

template<typename _Exec, typename _Tasks>
void BaseThreadPool<_Exec, _Tasks>::stop()
{
    _tasks->stop();
    // _notify_all();
}

template<typename _Exec, typename _Tasks>
void BaseThreadPool<_Exec, _Tasks>::wait_done()
{
    for (auto& exec : _execs) {
        exec.wait_task();
    }
}

template<typename _Exec, typename _Tasks>
void BaseThreadPool<_Exec, _Tasks>::_init_executors(size_t size)
{
    _execs.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        _execs.emplace_back(_tasks);
    }
    _execs.shrink_to_fit();
}

} // namespace cperf
