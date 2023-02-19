#include "thread_pool.h"

#include <iostream>

namespace utils
{
    
template<typename _Exec>
BaseThreadPool<_Exec>::BaseThreadPool(size_t size) :
    _tasks(std::make_shared<SyncTasksState>())
{
    _init_executors(size);
}

template<typename _Exec>
BaseThreadPool<_Exec>::~BaseThreadPool()
{
    _tasks->stop();
    for (auto& t : _execs) {
        t.join();
    }
}

template<typename _Exec>
void BaseThreadPool<_Exec>::push(TaskType task)
{
    _tasks->push(task);
    // _notify_all();
}

template<typename _Exec>
void BaseThreadPool<_Exec>::start()
{
    _tasks->start();
    _notify_all();
}

template<typename _Exec>
void BaseThreadPool<_Exec>::pause()
{
    _tasks->pause();
    // _notify_all();
}

template<typename _Exec>
void BaseThreadPool<_Exec>::stop()
{
    _tasks->stop();
    _notify_all();
}

template<typename _Exec>
void BaseThreadPool<_Exec>::wait_done()
{
    // for (auto& exec : _execs) {
    //     std::cout << exec._thr.get_id() << ":" << exec._has_waited.load() << "\n";
    // }
    for (auto& exec : _execs) {
        exec.wait_task();
    }
}
    
template<typename _Exec>
void BaseThreadPool<_Exec>::_notify_all()
{
    for (auto& exec : _execs) {
        exec.notify();
    }
}

template<typename _Exec>
void BaseThreadPool<_Exec>::_init_executors(size_t size)
{
    _execs.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        _execs.emplace_back(_tasks);
    }
    // _execs.shrink_to_fit();
}

} // namespace cperf
