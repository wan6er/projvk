#include "thread_executor.h"

namespace utils
{

ThreadExecutor::ThreadExecutor(ThreadExecutor const& exec) : 
    BaseThreadExecutor(exec) 
{}
    
void ThreadExecutor::_wait_pause()
{
    _wait([this]() { return _state->get_state() != ThreadState::PAUSE; });
}

auto ThreadExecutor::operator=(ThreadExecutor const& exec) -> ThreadExecutor&
{
    BaseThreadExecutor::operator=(exec);
    return *this;
}

} // namespace utils

