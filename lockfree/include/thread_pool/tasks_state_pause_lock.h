#ifndef __PAUSE_LOCK_TASKS_STATE_H__
#define __PAUSE_LOCK_TASKS_STATE_H__

#include "clf_basic.h"
#include "tasks_state.h"

#include <mutex>
#include <condition_variable>

namespace utils
{

class CLF_API TasksStatePauseLock : public TasksState
{
public:
    using _BaseType = TasksState;
    
    virtual void start();
    virtual void pause();
    virtual void stop();
    virtual void wait_finish();

    // virtual void wait_finish_done();

    void signal_pause();
    void wait_pause();
    // void wait_task();
    // void signal_task();

protected:
    std::mutex _pause_mtx;
    std::condition_variable _pause_cv;

};

} // namespace cperf

#endif