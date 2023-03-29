#include "thread_pool/thread_pool.h"
#include "thread_pool.inl"

namespace utils
{

template class CLF_API BaseThreadPool<ThreadExecutor, ThreadExecutor::TasksStateType>;
// template class CLF_API BaseThreadPool<ThreadExecutor>;
// template class CLF_API BaseThreadPool<LockFreeThreadExecutor, LockFreeThreadExecutor::TasksStateType>;

} // namespace utils

