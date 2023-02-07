#pragma once

namespace utils
{
    
// template<typename _Type, typename..._Args>
// template<typename __Head, typename...__Tail>
// constexpr _TupleImpl<_Type, _Args...>::_TupleImpl(__Head&& head, __Tail&&...tail) :
//     _BaseType(std::forward<__Tail>(tail)...), _obj(std::forward<__Head>(head))
// {
// }

// template<typename _Type, typename..._Args>
// constexpr _TupleImpl<_Type, _Args...>::_TupleImpl(_Type CONST_REFERENCE obj, _Args CONST_REFERENCE...args) :
//     _BaseType(args...), _obj(obj)
// {
// }

template<typename _Type, typename..._Args>
inline constexpr auto _TupleImpl<_Type, _Args...>::get_data() -> _FirstType&
{
    return _first._val;
}

template<typename _Type, typename..._Args>
inline constexpr auto _TupleImpl<_Type, _Args...>::get_data() const -> _FirstType CONST_REFERENCE
{
    return _first._val;
}


template<size_t..._Ti, typename _Tuple>
inline constexpr auto _to_std_impl(IndexSequence<_Ti...>, _Tuple CONST_REFERENCE tuple)
{
    return std::tuple(get<_Ti>(tuple)...);
}

template<typename..._Elements>
inline constexpr auto Tuple<_Elements...>::to_std() const -> std::tuple<_Elements...>
{
    return _to_std_impl(MakeSequenceByTupleSize<TupleType>(), *this);
}


} // namespace utils
