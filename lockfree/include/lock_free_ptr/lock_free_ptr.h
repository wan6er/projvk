#pragma once

#include "lock_free_ptr_base.h"
#include "lock_free_weak_ptr_base.h"
#include "shared_ptr.h"
#include "memory_manager.h"

namespace utils
{
    
template<typename _Ty>
struct LockFreeWeakPtr;

template<typename _Ty>
struct LockFreePtr : public BaseLockFreePtr<_Ty, SharedPtr<_Ty>, CountObjR<_Ty>, MemoryManager<_Ty, CountObjR<_Ty>>>
{
    using Type = _Ty;
    using AtomicWeakPtr = LockFreeWeakPtr<_Ty>;
    using _CountObj = CountObjR<_Ty>;
    using SharedPtrType = SharedPtr<_Ty>;
    using _Base = BaseLockFreePtr<_Ty, SharedPtr<_Ty>, _CountObj, MemoryManager<_Ty, _CountObj>>;
    using _Base::_Base;
  
    void operator=(LockFreePtr const& ptr);

    auto get(MemoryOrder order = MemoryOrderAcquire) -> SharedPtrType;
    auto get(MemoryOrder order = MemoryOrderAcquire) const -> SharedPtrType;
    
    template<typename __Ptr>
    constexpr auto operator==(__Ptr&& ptr) const -> bool;
    template<typename __Ptr>
    constexpr auto operator!=(__Ptr&& ptr) const -> bool;
    // constexpr operator bool() const;
    // auto get_snapshot() const -> AtomicWeakPtr { return AtomicWeakPtr(new _CountObj(this->load_count())); }

    friend class LockFreeWeakPtr<_Ty>;
};

template<typename _Ty>
struct LockFreeWeakPtr : public BaseLockFreeWeakPtr<_Ty, WeakPtr<_Ty>, CountObjR<_Ty>, MemoryManager<_Ty, CountObjR<_Ty>>>
{
    using Type = _Ty;
    using AtomicSharedPtr = LockFreePtr<_Ty>;
    using AtomicWeakPtr = LockFreeWeakPtr;
    using _CountObj = CountObjR<_Ty>;
    using WeakPtrType = WeakPtr<_Ty>;
    using _Base = BaseLockFreeWeakPtr<_Ty, WeakPtr<_Ty>, _CountObj, MemoryManager<_Ty, _CountObj>>;
    using _Base::_Base;

    constexpr LockFreeWeakPtr() = default;
    constexpr LockFreeWeakPtr(AtomicSharedPtr const& shared_ptr);

    constexpr auto operator=(AtomicSharedPtr const& ptr) -> AtomicWeakPtr&;
    constexpr auto operator=(AtomicWeakPtr const& ptr) -> AtomicWeakPtr&;

    auto get(MemoryOrder order = MemoryOrderAcquire) const -> WeakPtrType;
    
    template<typename __Ptr>
    constexpr auto operator==(__Ptr&& ptr) const -> bool;
    template<typename __Ptr>
    constexpr auto operator!=(__Ptr&& ptr) const -> bool;
    // constexpr operator bool() const;

    constexpr auto get_shared() const -> AtomicSharedPtr;

};

template<typename __Ty, typename...__Args>
auto make_ptr(__Args&&...args) -> LockFreePtr<__Ty>;

} // namespace utils

#include "lock_free_ptr.inl"
