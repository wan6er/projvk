#pragma once

#include <type_traits>

namespace utils
{

template<class __Type>
auto remove_clr() -> std::remove_const_t<std::remove_reference_t<__Type>>;

template<class _Type>
void BaseSingleAttachWrapper<_Type>::attach(_Type CONST_REFERENCE attachment)
{
    _attachment = attachment;
}

template<class _Type>
BaseSingleAttachWrapper<_Type>::operator _Type& ()
{
    return _attachment;
}

template<class _Type>
BaseSingleAttachWrapper<_Type>::operator _Type CONST_REFERENCE () const
{
    return _attachment;
}

template<class..._Args>
template<class __Type>
void BaseSingleAttach<_Args...>::attach(__Type&& attachment)
{
    // decltype(remove_clr<__Type>()) a;
    BaseSingleAttachWrapper<decltype(remove_clr<__Type>())>::attach(attachment);
}

};