#ifndef __LOCK_FREE_PTR_IMPL_H__
#define __LOCK_FREE_PTR_IMPL_H__

#include "compatible_atomic.h"
#include "memory_manager.h"

namespace utils
{

template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
class BaseLockFreePtrImpl : public std::enable_if_t<std::is_base_of_v<BaseMemoryManager, _MemoryManager>, _MemoryManager>
{
    using ValPtr = _Ty*;
    using ConstValPtr = _Ty const*;
    using CountPtr = _CountObj*;
    using _AtomicCountObj = Atomic<CountPtr>;

public:
    
    BaseLockFreePtrImpl();
    BaseLockFreePtrImpl(CountPtr ptr);
    BaseLockFreePtrImpl(BaseLockFreePtrImpl const& ptr);
    virtual ~BaseLockFreePtrImpl();

    auto load_count(MemoryOrder order = MemoryOrderRelaxed) const -> CountPtr;
    void store_count(CountPtr count, MemoryOrder order = MemoryOrderRelaxed);
    auto empty() const -> bool { return load_count() == nullptr; }
    

protected:
    _AtomicCountObj _obj;

};

} // namespace utils

#include "lock_free_ptr_impl.inl"

#endif