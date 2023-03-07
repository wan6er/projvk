
#include <assert.h>

namespace utils
{

template<typename _Ty, typename..._Args>
auto _new_obj(_Args&&...args) -> _Ty*
{
    return new _Ty(std::forward<_Args>(args)...);
}

template<typename _Ty>
auto _new_obj() -> _Ty*
{
    return new _Ty;
}

template<typename _Ty>
void _del_obj(_Ty*& val)
{
    _Ty* tmp = val;
    val = nullptr;
    std::atomic_thread_fence(std::memory_order_acquire);
    delete tmp;
}

template<typename _Ty>
SyncRef<_Ty>::SyncRef() :
    _ptr(_new_obj<_Ty>()),
    _count(_new_obj<_RefCount>(1))
{
}

template<typename _Ty>
SyncRef<_Ty>::SyncRef(Type const& val) :
    _ptr(nullptr),
    _count(nullptr)
{
    CPERF_ASSERT((val._count != nullptr && val._ptr != nullptr));
    if (val._count->fetch_add(1, std::memory_order_acq_rel)) {
        _count = val._count;
        _ptr = val._ptr;
    }
}

template<typename _Ty>
SyncRef<_Ty>::~SyncRef()
{
    if (_count->load(std::memory_order_release) == 1) {
        _del_obj(_ptr);
        _del_obj(_count);
    } else {
        unref();
    }
}

} // namespace utils
