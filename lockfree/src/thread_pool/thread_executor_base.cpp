#include "thread_pool/thread_executor_base.h"
#include "thread_executor_base.inl"

namespace utils
{

template class CLF_API BaseThreadExecutorImpl<TasksState>;
template class CLF_API BaseThreadExecutorImpl<TasksStatePauseLock>;

}; // namespace cperf
