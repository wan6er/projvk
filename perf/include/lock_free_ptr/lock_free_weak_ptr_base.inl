
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


// template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
// constexpr auto BaseLockFreeWeakPtr<_Ty, _Derived, _CountObj, _MemoryManager>::load() const -> CountObjPtr
// {
//     return _obj.load(std::memory_order_relaxed); 
// }

// template<typename _Ty, typename _Derived, typename _CountObj, typename _MemoryManager>
// constexpr void BaseLockFreeWeakPtr<_Ty, _Derived, _CountObj, _MemoryManager>::store(CountObjPtr ptr)
// {
//     _obj.store(ptr, std::memory_order_relaxed);
// }


} // namespace utils
