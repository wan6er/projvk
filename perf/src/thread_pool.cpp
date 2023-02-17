#include "thread_pool.h"

#include <iostream>

namespace utils
{
    
ThreadPool::ThreadPool(size_t size) :
    _tasks(std::make_shared<SyncTasksState>())
{
    _init_executors(size);
}

ThreadPool::~ThreadPool()
{
    _tasks->stop();
    for (auto& t : _execs) {
        t.join();
    }
}

void ThreadPool::push(TaskType task)
{
    _tasks->push(task);
    _notify_all();
}

void ThreadPool::start()
{
    _tasks->start();
    _notify_all();
}

void ThreadPool::pause()
{
    _tasks->pause();
    _notify_all();
}

void ThreadPool::stop()
{
    _tasks->stop();
    _notify_all();
}

void ThreadPool::wait_done()
{
    // for (auto& exec : _execs) {
    //     std::cout << exec._thr.get_id() << ":" << exec._has_waited.load() << "\n";
    // }
    for (auto& exec : _execs) {
        exec.wait_task();
    }
}
    
void ThreadPool::_notify_all()
{
    for (auto& exec : _execs) {
        exec.notify();
    }
}

void ThreadPool::_init_executors(size_t size)
{
    _execs.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        _execs.emplace_back(_tasks);
    }
    // _execs.shrink_to_fit();
}

} // namespace cperf
