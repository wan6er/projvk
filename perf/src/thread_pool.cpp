#include "thread_pool.h"
#include "thread_pool.inl"

namespace utils
{

template class CPERF_API BaseThreadPool<ThreadExecutor, ThreadExecutor::TasksStateType>;
// template class CPERF_API BaseThreadPool<ThreadExecutor>;
template class CPERF_API BaseThreadPool<LockFreeThreadExecutor, LockFreeThreadExecutor::TasksStateType>;

} // namespace utils

