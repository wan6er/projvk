#include "thread_executor_base.h"
#include "thread_executor_base.inl"

namespace utils
{

template class CPERF_API BaseThreadExecutorImpl<TasksState>;
template class CPERF_API BaseThreadExecutorImpl<TasksStatePauseLock>;

}; // namespace cperf
