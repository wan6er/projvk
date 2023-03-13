#pragma once

#include "lock_free_ptr_base.h"
#include "lock_free_weak_ptr_base.h"
#include "count_obj.h"
#include "memory_manager.h"

namespace utils
{
    
template<typename _Ty>
struct LockFreeWeakPtr;

template<typename _Ty>
struct LockFreePtr : public BaseLockFreePtr<_Ty, CountObj<_Ty>, MemoryManager<_Ty>>
{
    using Type = _Ty;
    using _BaseType = BaseLockFreePtr<_Ty, CountObj<_Ty>, MemoryManager<_Ty>>;
    using _BaseType::_BaseType;
    
    void operator=(LockFreePtr const& ptr);

    constexpr auto operator*() -> Type&;
    constexpr auto operator*() const -> Type const&;

    constexpr auto operator->() -> Type*;
    constexpr auto operator->() const -> Type const*;
    
    constexpr auto operator==(void* ptr) const -> bool;
    constexpr operator bool() const;

    friend class LockFreeWeakPtr<_Ty>;
};

template<typename _Ty>
struct LockFreeWeakPtr : public BaseLockFreeWeakPtr<_Ty, CountObj<_Ty>, MemoryManager<_Ty>>
{
    using _BaseType = BaseLockFreeWeakPtr<_Ty, CountObj<_Ty>, MemoryManager<_Ty>>;

    using SharedPtr = LockFreePtr<_Ty>;
    using WeakPtr = LockFreeWeakPtr;
    using Type = _Ty;

    using _BaseType::_BaseType;

    constexpr LockFreeWeakPtr(SharedPtr const& shared_ptr);

    constexpr auto operator=(SharedPtr const& ptr) -> WeakPtr&;
    constexpr auto operator=(WeakPtr const& ptr) -> WeakPtr&;

    constexpr auto operator*() -> Type&;
    constexpr auto operator*() const -> Type const&;

    constexpr auto operator->() -> Type*;
    constexpr auto operator->() const -> Type const*;

    constexpr auto operator==(void* ptr) const -> bool;
    constexpr operator bool() const;

    constexpr auto get_shared() const -> SharedPtr;

};

template<typename __Ty, typename...__Args>
auto make_ptr(__Args&&...args) -> LockFreePtr<__Ty>;


} // namespace utils

#include "lock_free_ptr.inl"
