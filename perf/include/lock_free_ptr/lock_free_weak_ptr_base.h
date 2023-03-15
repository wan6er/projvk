#ifndef __LOCK_FREE_WEAK_PTR_BASE_H__
#define __LOCK_FREE_WEAK_PTR_BASE_H__

#include "lock_free_ptr_impl.h"

namespace utils
{
    
template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
class BaseLockFreeWeakPtr : 
    public BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>
{
public:
    using Base = BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>;
    using CountObjPtr = _CountObj*;
    using MemoryManager = _MemoryManager;
    using _AtomicCountObj = Atomic<CountObjPtr>;

    constexpr BaseLockFreeWeakPtr();
    constexpr BaseLockFreeWeakPtr(BaseLockFreeWeakPtr const& ptr);
    virtual ~BaseLockFreeWeakPtr();

protected:
    // constexpr auto load() const -> CountObjPtr;
    // constexpr void store(CountObjPtr ptr);

protected:
    // _AtomicCountObj _obj;

};


} // namespace utils

#include "lock_free_weak_ptr_base.inl"

#endif