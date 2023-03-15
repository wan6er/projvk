
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


} // namespace utils
