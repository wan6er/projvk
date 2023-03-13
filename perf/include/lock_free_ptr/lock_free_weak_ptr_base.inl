
namespace utils
{
    
template<typename _Ty, typename _CountObj, typename _MemoryManager>
constexpr BaseLockFreeWeakPtr<_Ty, _CountObj, _MemoryManager>::BaseLockFreeWeakPtr() :
    _obj(nullptr)
{
}

// template<typename _Ty, typename _CountObj, typename _MemoryManager>
// constexpr BaseLockFreeWeakPtr<_Ty, _CountObj, _MemoryManager>::BaseLockFreeWeakPtr(SharedObjType const& ptr) :
//     _obj(ptr.get_count_obj())
// {
// }

template<typename _Ty, typename _CountObj, typename _MemoryManager>
constexpr BaseLockFreeWeakPtr<_Ty, _CountObj, _MemoryManager>::BaseLockFreeWeakPtr(BaseLockFreeWeakPtr const& ptr) :
    _obj(ptr.get_count_obj().load())
{
}

template<typename _Ty, typename _CountObj, typename _MemoryManager>
BaseLockFreeWeakPtr<_Ty, _CountObj, _MemoryManager>::~BaseLockFreeWeakPtr()
{
}


template<typename _Ty, typename _CountObj, typename _MemoryManager>
constexpr auto BaseLockFreeWeakPtr<_Ty, _CountObj, _MemoryManager>::load() const -> CountObjPtr
{
    return _obj.load(std::memory_order_relaxed); 
}

template<typename _Ty, typename _CountObj, typename _MemoryManager>
constexpr void BaseLockFreeWeakPtr<_Ty, _CountObj, _MemoryManager>::store(CountObjPtr ptr)
{
    _obj.store(ptr, std::memory_order_relaxed);
}


} // namespace utils
