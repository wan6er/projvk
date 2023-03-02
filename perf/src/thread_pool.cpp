#include "thread_pool.h"
#include "thread_pool.inl"

namespace utils
{

template class CPERF_API BaseThreadPool<BaseThreadExecutor>;
template class CPERF_API BaseThreadPool<LockThreadExecutor>;
template class CPERF_API BaseThreadPool<LockFreeThreadExecutor>;

} // namespace utils

