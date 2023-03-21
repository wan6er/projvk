
namespace utils
{

template<typename _Ty, typename _CountObj, typename _Derived>
constexpr BaseSharedPtrImpl<_Ty, _CountObj, _Derived>::BaseSharedPtrImpl() :
    _obj(nullptr)
{
}

template<typename _Ty, typename _CountObj, typename _Derived>
constexpr BaseSharedPtrImpl<_Ty, _CountObj, _Derived>::BaseSharedPtrImpl(_CountObj ptr) :
    _obj(ptr)
{
}

template<typename _Ty, typename _CountObj, typename _Derived>
constexpr BaseSharedPtrImpl<_Ty, _CountObj, _Derived>::BaseSharedPtrImpl(BaseSharedPtrImpl const& ptr) :
    _obj(ptr._obj)
{
}

template<typename _Ty, typename _CountObj, typename _Derived>
BaseSharedPtrImpl<_Ty, _CountObj, _Derived>::~BaseSharedPtrImpl()
{
}

template<typename _Ty, typename _CountObj, typename _Derived>
constexpr auto BaseSharedPtrImpl<_Ty, _CountObj, _Derived>::get_count_ref() -> _CountObj&
{
    return _obj;
}

template<typename _Ty, typename _CountObj, typename _Derived>
constexpr auto BaseSharedPtrImpl<_Ty, _CountObj, _Derived>::get_count() const -> _CountObj
{
    return _obj;
}

} // namespace utils
