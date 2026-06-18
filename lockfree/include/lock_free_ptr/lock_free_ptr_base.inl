

namespace utils
{
    
template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::BaseLockFreePtr()
{
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::BaseLockFreePtr(_CountObj ptr) :
    Base(ptr)
{
    auto cnt = this->load_count(MemoryOrderRelaxed);
    this->increment(cnt);
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::BaseLockFreePtr(void* ptr) :
    BaseLockFreePtr(_CountObj(ptr))
{
    // auto cnt = this->load_count(MemoryOrderRelaxed);
    // this->increment(cnt);
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::BaseLockFreePtr(BaseLockFreePtr const& ptr) :
    Base(ptr)
{
    auto _count = this->load_count(std::memory_order_relaxed);
    this->increment(_count);
    
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::~BaseLockFreePtr()
{
    auto _count = this->load_count(std::memory_order_relaxed);
    this->decrement(_count);
    this->store_count(_CountObj{}, std::memory_order_acquire);
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
void BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::load(_Derived& ptr, MemoryOrder order) 
{ 
    // std::lock_guard<std::mutex> locker(mtx);
    auto cnt_obj = this->load_count(order);
    read_cnt_add(cnt_obj);
    ptr.swap(cnt_obj);
    read_cnt_sub(cnt_obj);
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
void BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::store(_Derived ptr, MemoryOrder order)
{
    _Derived _obj;
    load(_obj);
    while (!compare_exchange_weak(_obj, ptr));
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
bool BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::compare_exchange_weak(_Derived& expected, _Derived const& desired, MemoryOrder order)
{
    if (compare_swap_impl(expected, desired, order)) {
        return true;
    }
    load(expected);
    return false;
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
bool BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::compare_exchange_strong(_Derived const& expected, _Derived const& desired, MemoryOrder order)
{
    return compare_swap_impl(expected, desired, order);
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
bool BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::compare_swap_impl(_Derived const& expected, _Derived const& desired, MemoryOrder order)
{
    auto d_cnt = desired.get_count().add_cnt();
    auto e_cnt = expected.get_count();
    
    if (e_cnt.r_cnt == 0 && this->_obj.compare_exchange_strong(e_cnt, d_cnt, order)) {
        this->increment(d_cnt);
        read_cnt_sub(d_cnt);
        this->decrement(e_cnt);
        return true;
    } 
    return false;
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
void BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::read_cnt_add(_CountObj& cnt)
{
    while (!this->_obj.compare_exchange_weak(cnt, cnt.add_cnt()));
}

template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
void BaseLockFreePtr<_Ty, _Derived, _CountObj, _MemoryManager>::read_cnt_sub(_CountObj& cnt)
{
    while (!this->_obj.compare_exchange_weak(cnt, cnt.sub_cnt()));
}


} // namespace utils
