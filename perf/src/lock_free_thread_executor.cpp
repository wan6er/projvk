#include "lock_free_thread_executor.h"

namespace utils
{
    
void LockFreeThreadExecutor::notify()
{
}
    
void LockFreeThreadExecutor::_wait_pause()
{
    _wait([this]() { return _state->get_state() != ThreadState::PAUSE; });
}

void LockFreeThreadExecutor::_wait_task_push()
{
    _wait([this]() { return _state->size() > 0 || _state->get_state() != ThreadState::RUNNING; });
}


} // namespace utils

