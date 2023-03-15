#ifndef __LOCK_FREE_PTR_IMPL_H__
#define __LOCK_FREE_PTR_IMPL_H__

#include "compatible_atomic.h"
#include "memory_manager.h"

namespace utils
{

template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
class BaseLockFreePtrImpl : public std::enable_if_t<std::is_base_of_v<BaseMemoryManager, _MemoryManager>, _MemoryManager>
{
    // static_assert(std::is_base_of_v<BaseLockFreePtrImpl<_Ty, _CountObj, _Derived>, _Derived>);

public:
    using ValPtr = _Ty*;
    using ConstValPtr = _Ty const*;
    using CountPtr = _CountObj*;
    using _AtomicCountObj = Atomic<CountPtr>;
    
    constexpr BaseLockFreePtrImpl();
    constexpr BaseLockFreePtrImpl(CountPtr ptr);
    constexpr BaseLockFreePtrImpl(BaseLockFreePtrImpl const& ptr);
    virtual ~BaseLockFreePtrImpl();

    constexpr auto load(MemoryOrder order = MemoryOrderRelaxed) const -> _Derived;
    constexpr void store(_Derived ptr, MemoryOrder order = MemoryOrderRelaxed);
    constexpr bool compare_exchange_weak(_Derived& expected, _Derived const& desired, MemoryOrder order = MemoryOrderRelaxed);
    constexpr bool compare_exchange_strong(_Derived const& expected, _Derived const& desired, MemoryOrder order = MemoryOrderRelaxed) volatile;

    constexpr auto load_count(MemoryOrder order = MemoryOrderRelaxed) const -> CountPtr;
    constexpr void store_count(CountPtr count, MemoryOrder order = MemoryOrderRelaxed);
    
protected:
    // constexpr bool compare_exchange_weak_impl(CountPtr& expected, CountPtr desired, MemoryOrder order = MemoryOrderRelaxed) volatile;
    constexpr bool compare_swap_impl(CountPtr expected, CountPtr desired, MemoryOrder order = MemoryOrderRelaxed);

protected:
    _AtomicCountObj _obj;

};

} // namespace utils

#include "lock_free_ptr_impl.inl"

#endif