#pragma once


namespace utils
{

template<typename _Type, typename..._Elements>
inline constexpr auto TupleUnion<_Type, _Elements...>::operator->() -> _Type*
{
    return &data;
}

template<typename _Type, typename..._Elements>
inline constexpr auto TupleUnion<_Type, _Elements...>::operator->() const -> _Type CONST_PTR
{
    return &data;
}

} // namespace utils
