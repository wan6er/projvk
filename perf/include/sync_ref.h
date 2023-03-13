#ifndef __SYNC_REF_H__
#define __SYNC_REF_H__

#include "perf_basic.h"

#include <stdint.h>
#include <atomic>

namespace utils
{

template<typename _Ty, typename..._Args>
auto _new_obj(_Args&&...args) -> _Ty*;

template<typename _Ty>
auto _new_obj() -> _Ty*;

template<typename _Ty>
void _del_obj(_Ty*& val);
    
template<typename _Ty>
class SyncRef
{
public:
    using Type = SyncRef<_Ty>;
    using ValType = _Ty;
    using _RefCount = std::atomic<uint32_t>;
    using _RefCountPtr = _RefCount*;
    using _ValPtrType = ValType*;

    SyncRef();
    SyncRef(Type const&);

    template<typename __First, typename...__Args>
    SyncRef(std::enable_if_t<!std::is_same_v<__First, Type>, __First>&& first, __Args&&...args) :
        _ptr(_new_obj<_Ty>(std::forward<__Args>(args)...)),
        _count(_new_obj<_RefCount>(1))
    {}

    virtual ~SyncRef();

    void ref() { _count->fetch_add(1, std::memory_order_acq_rel); }
    void unref()  { _count->fetch_sub(1, std::memory_order_acq_rel); }
    uint32_t count() const { return _count->load(std::memory_order_relaxed); }

    auto operator->() -> ValType* { return _ptr; }
    auto operator->() const -> ValType const* { return _ptr; }

private:
    _ValPtrType _ptr; 
    _RefCountPtr _count;

};

} // namespace utils

#include "sync_ref.inl"

#endif