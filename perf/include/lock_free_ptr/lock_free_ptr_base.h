#ifndef __LOCK_FREE_PTR_BASE_H__
#define __LOCK_FREE_PTR_BASE_H__

#include "lock_free_ptr_impl.h"

namespace utils
{
    
template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
class BaseLockFreePtr : 
    public BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>
{
public:
    using Base = BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>;
    using ObjType = BaseLockFreePtr;

    BaseLockFreePtr();
    BaseLockFreePtr(_CountObj ptr);
    BaseLockFreePtr(void* ptr);
    BaseLockFreePtr(BaseLockFreePtr const& ptr);
    virtual ~BaseLockFreePtr();

    void load(_Derived& ptr, MemoryOrder order = MemoryOrderRelaxed);
    void store(_Derived ptr, MemoryOrder order = MemoryOrderRelaxed);
    bool compare_exchange_weak(_Derived& expected, _Derived const& desired, MemoryOrder order = MemoryOrderRelaxed);
    bool compare_exchange_strong(_Derived const& expected, _Derived const& desired, MemoryOrder order = MemoryOrderRelaxed);

protected:
    void read_cnt_add(_CountObj& cnt);
    void read_cnt_sub(_CountObj& cnt);

    bool compare_swap_impl(_Derived const& expected, _Derived const& desired, MemoryOrder order = MemoryOrderRelaxed);

protected:

};


} // namespace utils

#include "lock_free_ptr_base.inl"

#endif