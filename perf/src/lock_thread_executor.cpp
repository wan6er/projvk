#include "lock_thread_executor.h"

namespace utils
{

void LockThreadExecutor::_wait_task_push()
{
    // _wait([this]() { return _state->get_queue().size() > 0 || _state->get_state() != ThreadState::RUNNING; });
    _state->wait_task();
}


} // namespace utils

