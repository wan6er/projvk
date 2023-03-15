
namespace utils
{
    
template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
constexpr BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::BaseLockFreePtr() :
    Base()
{
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
constexpr BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::BaseLockFreePtr(CountPtr ptr) :
    Base(ptr)
{
    this->increment(this->load_count(std::memory_order_relaxed));
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
constexpr BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::BaseLockFreePtr(BaseLockFreePtr const& ptr) :
    Base(ptr)
{
    auto _count = this->load_count(std::memory_order_relaxed);
    this->increment(_count);
    
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::~BaseLockFreePtr()
{
    auto _count = this->load_count(std::memory_order_relaxed);
    if (this->decrement(_count)) {
        auto tmp = _count;
        this->store_count(nullptr, std::memory_order_acquire);
        this->destroy(tmp);
    }
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
constexpr auto BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::load(MemoryOrder order) const -> _Derived 
{ 
    return _Derived(this->load_count(order));
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
constexpr void BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::store(_Derived ptr, MemoryOrder order)
{
    store_count(ptr.get_count(), order);
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
constexpr bool BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::compare_exchange_weak(_Derived& expected, _Derived const& desired, MemoryOrder order)
{
    if (!compare_swap_impl(expected.get_count(), desired.get_count(), order)) {
        expected = load();
        return false;
    }
    return true;
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
constexpr bool BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::compare_exchange_strong(_Derived const& expected, _Derived const& desired, MemoryOrder order) volatile
{
    return compare_swap_impl(expected.get_count(), desired.get_count(), order);
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
constexpr bool BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::compare_swap_impl(CountPtr expected, CountPtr desired, MemoryOrder order)
{
    if (this->_obj.compare_exchange_strong(expected, desired, order)) {
        this->increment(desired);
        this->decrement(expected);
        return true;
    }
    return false;
}



} // namespace utils
