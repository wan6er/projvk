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
struct LockFreePtr : public BaseLockFreePtr<_Ty, SharedPtr<_Ty>, CountObj<_Ty>, MemoryManager<_Ty>>
{
    using Type = _Ty;
    using _Base = BaseLockFreePtr<_Ty, SharedPtr<_Ty>, CountObj<_Ty>, MemoryManager<_Ty>>;
    using _Base::_Base;
  
    void operator=(LockFreePtr const& ptr);

    constexpr auto operator*() -> Type&;
    constexpr auto operator*() const -> Type const&;

    constexpr auto operator->() -> Type*;
    constexpr auto operator->() const -> Type const*;
    
    template<typename __Ptr>
    constexpr auto operator==(__Ptr&& ptr) const -> bool;
    template<typename __Ptr>
    constexpr auto operator!=(__Ptr&& ptr) const -> bool;
    // constexpr operator bool() const;

    friend class LockFreeWeakPtr<_Ty>;
};

template<typename _Ty>
struct LockFreeWeakPtr : public BaseLockFreeWeakPtr<_Ty, WeakPtr<_Ty>, CountObj<_Ty>, MemoryManager<_Ty>>
{
    using Type = _Ty;
    using AtomicSharedPtr = LockFreePtr<_Ty>;
    using AtomicWeakPtr = LockFreeWeakPtr;
    using _Base = BaseLockFreeWeakPtr<_Ty, WeakPtr<_Ty>, CountObj<_Ty>, MemoryManager<_Ty>>;
    using _Base::_Base;

    constexpr LockFreeWeakPtr() = default;
    constexpr LockFreeWeakPtr(AtomicSharedPtr const& shared_ptr);

    constexpr auto operator=(AtomicSharedPtr const& ptr) -> AtomicWeakPtr&;
    constexpr auto operator=(AtomicWeakPtr const& ptr) -> AtomicWeakPtr&;

    constexpr auto operator*() -> Type&;
    constexpr auto operator*() const -> Type const&;

    constexpr auto operator->() -> Type*;
    constexpr auto operator->() const -> Type const*;

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
