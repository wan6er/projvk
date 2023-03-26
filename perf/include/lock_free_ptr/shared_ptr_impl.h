#ifndef __SHARED_PTR_IMPL_H__
#define __SHARED_PTR_IMPL_H__

#include "memory_manager_base.h"

namespace utils
{

template<typename _Ty, typename _CountObj, typename _Derived>
class BaseSharedPtrImpl
{
public:
    using ValPtr = _Ty*;
    using ConstValPtr = _Ty const*;
    
    constexpr BaseSharedPtrImpl();
    constexpr BaseSharedPtrImpl(_CountObj& ptr);
    constexpr BaseSharedPtrImpl(BaseSharedPtrImpl const& ptr);

    constexpr auto get_count_ref() -> _CountObj&;
    constexpr auto get_count_ref() const -> _CountObj const&;
    constexpr auto get_count() const -> _CountObj;
    // constexpr void store_count(_CountObj count);

protected:
    _CountObj _obj;

};

} // namespace utils

#include "shared_ptr_impl.inl"

#endif