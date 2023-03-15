#pragma once

#include "shared_ptr_impl.h"
#include "count_obj.h"
#include "memory_manager.h"

namespace utils
{

    
template<typename _Ty>
struct WeakPtr;
    
template<typename _Ty>
struct SharedPtr :  
    public BaseSharedPtrImpl<_Ty, CountObj<_Ty>, SharedPtr<_Ty>>, 
    public MemoryManager<_Ty>
{
    using Type = _Ty;
    using _CountPtr = CountObj<_Ty>*;
    using _Base = BaseSharedPtrImpl<_Ty, CountObj<_Ty>, SharedPtr<_Ty>>;
    // using _BaseType::_BaseType;

    constexpr SharedPtr();
    constexpr SharedPtr(_CountPtr count);
    constexpr SharedPtr(SharedPtr const& ptr);
    virtual ~SharedPtr();
    
    void operator=(SharedPtr const& ptr);

    constexpr auto operator*() -> Type&;
    constexpr auto operator*() const -> Type const&;

    constexpr auto operator->() -> Type*;
    constexpr auto operator->() const -> Type const*;
    
    constexpr auto operator==(void* ptr) const -> bool;
    constexpr operator bool() const;

    friend class WeakPtr<_Ty>;
};

template<typename _Ty>
struct WeakPtr :  
    public BaseSharedPtrImpl<_Ty, CountObj<_Ty>, WeakPtr<_Ty>>, 
    public MemoryManager<_Ty>
{
    using Type = _Ty;
    using _SharedPtr = SharedPtr<_Ty>;
    using _BaseType = BaseSharedPtrImpl<_Ty, CountObj<_Ty>, WeakPtr<_Ty>>;
    using _BaseType::_BaseType;
    
    constexpr auto operator=(WeakPtr const& ptr) -> WeakPtr&;
    constexpr auto operator=(_SharedPtr const& ptr) -> WeakPtr&;

    constexpr auto operator*() -> Type&;
    constexpr auto operator*() const -> Type const&;

    constexpr auto operator->() -> Type*;
    constexpr auto operator->() const -> Type const*;
    
    constexpr auto operator==(void* ptr) const -> bool;
    constexpr operator bool() const;

    constexpr auto get_shared() const -> _SharedPtr;

};

template<typename __Ty, typename...__Args>
auto make_shared(__Args&&...args) -> SharedPtr<__Ty>;

} // namespace utils

#include "shared_ptr.inl"
