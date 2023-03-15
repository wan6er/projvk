
namespace utils
{
    
template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
constexpr BaseLockFreeWeakPtr<_Ty, _Derived, _CountObj, _MemoryManager>::BaseLockFreeWeakPtr() :
    Base(nullptr)
{
}

// template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
// constexpr BaseLockFreeWeakPtr<_Ty, _Derived, _CountObj, _MemoryManager>::BaseLockFreeWeakPtr(SharedObjType const& ptr) :
//     _obj(ptr.get_count_obj())
// {
// }

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
constexpr BaseLockFreeWeakPtr<_Ty, _Derived, _CountObj, _MemoryManager>::BaseLockFreeWeakPtr(BaseLockFreeWeakPtr const& ptr) :
    Base(ptr.load_count())
{
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
BaseLockFreeWeakPtr<_Ty, _Derived, _CountObj, _MemoryManager>::~BaseLockFreeWeakPtr()
{
}


template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
constexpr auto BaseLockFreeWeakPtr<_Ty, _Derived, _CountObj, _MemoryManager>::load(MemoryOrder order) const -> _Derived 
{ 
    return _Derived(this->load_count(order));
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
constexpr void BaseLockFreeWeakPtr<_Ty, _Derived, _CountObj, _MemoryManager>::store(_Derived ptr, MemoryOrder order)
{
    store_count(ptr.get_count(), order);
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
constexpr bool BaseLockFreeWeakPtr<_Ty, _Derived, _CountObj, _MemoryManager>::compare_exchange_weak(_Derived& expected, _Derived const& desired, MemoryOrder order)
{
    if (!compare_swap_impl(expected.get_count(), desired.get_count(), order)) {
        expected = load();
        return false;
    }
    return true;
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
constexpr bool BaseLockFreeWeakPtr<_Ty, _Derived, _CountObj, _MemoryManager>::compare_exchange_strong(_Derived const& expected, _Derived const& desired, MemoryOrder order) volatile
{
    return compare_swap_impl(expected.get_count(), desired.get_count(), order);
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
constexpr bool BaseLockFreeWeakPtr<_Ty, _Derived, _CountObj, _MemoryManager>::compare_swap_impl(CountPtr expected, CountPtr desired, MemoryOrder order)
{
    return this->_obj.compare_exchange_strong(expected, desired, order);
}

} // namespace utils
