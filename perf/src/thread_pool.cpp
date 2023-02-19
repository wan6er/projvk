#include "thread_pool.h"
#include "thread_pool.inl"

namespace utils
{

template class BaseThreadPool<BaseThreadExecutor>;
template class BaseThreadPool<LockThreadExecutor>;
template class BaseThreadPool<LockFreeThreadExecutor>;

} // namespace utils

