
namespace utils
{
    
template<typename _Ty, typename _CountObj, typename _MemoryManager>
constexpr BaseLockFreePtr<_Ty, _CountObj, _MemoryManager>::BaseLockFreePtr() :
    _obj(nullptr)
{
}

template<typename _Ty, typename _CountObj, typename _MemoryManager>
constexpr BaseLockFreePtr<_Ty, _CountObj, _MemoryManager>::BaseLockFreePtr(CountObjPtr ptr) :
    _obj(ptr)
{
    this->increment(_obj.load(std::memory_order_relaxed));
}

template<typename _Ty, typename _CountObj, typename _MemoryManager>
constexpr BaseLockFreePtr<_Ty, _CountObj, _MemoryManager>::BaseLockFreePtr(BaseLockFreePtr const& ptr) :
    _obj(ptr._obj.load())
{
    this->increment(_obj.load(std::memory_order_relaxed));
}

template<typename _Ty, typename _CountObj, typename _MemoryManager>
BaseLockFreePtr<_Ty, _CountObj, _MemoryManager>::~BaseLockFreePtr()
{
    if (this->decrement(_obj.load(std::memory_order_relaxed))) {
        auto tmp = _obj.load(std::memory_order_relaxed);
        _obj.store(nullptr, std::memory_order_acquire);
        this->destroy(tmp);
    }
}

template<typename _Ty, typename _CountObj, typename _MemoryManager>
constexpr auto BaseLockFreePtr<_Ty, _CountObj, _MemoryManager>::load() const -> CountObjPtr 
{ 
    return _obj.load(std::memory_order_relaxed); 
}

template<typename _Ty, typename _CountObj, typename _MemoryManager>
constexpr void BaseLockFreePtr<_Ty, _CountObj, _MemoryManager>::store(CountObjPtr ptr)
{
    _obj.store(ptr, std::memory_order_relaxed);
}

} // namespace utils
