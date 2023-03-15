
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
constexpr auto BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>::load_count(MemoryOrder order) const -> CountPtr
{
    return _obj.load(order);
}

template<typename _Ty, typename _CountObj, typename _Derived, typename _MemoryManager>
constexpr void BaseLockFreePtrImpl<_Ty, _CountObj, _Derived, _MemoryManager>::store_count(CountPtr ptr, MemoryOrder order)
{
    return _obj.store(ptr, order);
}



} // namespace utils
