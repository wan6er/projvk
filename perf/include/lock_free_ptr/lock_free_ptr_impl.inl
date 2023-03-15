
namespace utils
{

template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
constexpr BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>::BaseLockFreePtrImpl() :
    _obj(nullptr)
{
}

template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
constexpr BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>::BaseLockFreePtrImpl(CountPtr ptr) :
    _obj(ptr)
{
}

template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
constexpr BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>::BaseLockFreePtrImpl(BaseLockFreePtrImpl const& ptr) :
    _obj(ptr.load_count(MemoryOrderRelaxed))
{
}

template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>::~BaseLockFreePtrImpl()
{
}

template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
constexpr auto BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>::load(MemoryOrder order) const -> _Derived 
{ 
    return _Derived(load_count(order));
}

template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
constexpr void BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>::store(_Derived ptr, MemoryOrder order)
{
    store_count(ptr.get_count(), order);
}

template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
constexpr bool BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>::compare_exchange_weak(_Derived& expected, _Derived const& desired, MemoryOrder order)
{
    if (!compare_swap_impl(expected.get_count(), desired.get_count(), order)) {
        expected = load();
        return false;
    }
    return true;
}

template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
constexpr bool BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>::compare_exchange_strong(_Derived const& expected, _Derived const& desired, MemoryOrder order) volatile
{
    return compare_swap_impl(expected.get_count(), desired.get_count(), order);
}

template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
constexpr auto BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>::load_count(MemoryOrder order) const -> CountPtr
{
    return _obj.load(order);
}

template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
constexpr void BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>::store_count(CountPtr ptr, MemoryOrder order)
{
    return _obj.store(ptr, order);
}

// template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
// constexpr auto BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>::load_impl(MemoryOrder order) const volatile -> CountPtr 
// { 
//     return _obj.load(order);
// }

// template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
// constexpr void BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>::store_impl(CountPtr ptr, MemoryOrder order)
// {
//     _obj.store(ptr, order);
// }

// template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
// constexpr bool BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>::compare_exchange_weak_impl(CountPtr& expected, CountPtr desired, MemoryOrder order) volatile
// {
//     if (_obj.compare_exchange_strong_impl(expected, desired, order)) {
//         expected
//     }
// }

template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
constexpr bool BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>::compare_swap_impl(CountPtr expected, CountPtr desired, MemoryOrder order)
{
    if (_obj.compare_exchange_strong(expected, desired, order)) {
        this->increment(desired);
        this->decrement(expected);
        return true;
    }
    return false;
}


} // namespace utils
