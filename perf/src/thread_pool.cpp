#include "thread_pool.h"

namespace cperf
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
}

void ThreadPool::start()
{
    _tasks->start();
}

void ThreadPool::pause()
{
    _tasks->pause();
}

void ThreadPool::stop()
{
    _tasks->stop();
}

void ThreadPool::wait_done()
{
    _tasks->wait_all_tasks_done();
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
