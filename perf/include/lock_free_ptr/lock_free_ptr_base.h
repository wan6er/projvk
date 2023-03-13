#ifndef __LOCK_FREE_PTR_BASE_H__
#define __LOCK_FREE_PTR_BASE_H__

#include "compatible_atomic.h"
#include "memory_manager_base.h"

namespace utils
{
    
template<typename _Ty, typename _CountObj, typename _MemoryManager>
class BaseLockFreePtr : public std::enable_if_t<std::is_base_of_v<BaseMemoryManager, _MemoryManager>, _MemoryManager>
{
public:
    using ObjType = BaseLockFreePtr;
    using CountObjPtr = _CountObj*;
    using MemoryManager = _MemoryManager;
    using _AtomicCountObj = Atomic<CountObjPtr>;

    constexpr BaseLockFreePtr();
    constexpr BaseLockFreePtr(CountObjPtr ptr);
    constexpr BaseLockFreePtr(BaseLockFreePtr const& ptr);
    virtual ~BaseLockFreePtr();

protected:
    constexpr auto load() const -> CountObjPtr;
    constexpr void store(CountObjPtr ptr);

protected:
    _AtomicCountObj _obj;

};


} // namespace utils

#include "lock_free_ptr_base.inl"

#endif