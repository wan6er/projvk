#pragma once

#include <type_traits>
#include <utility>

namespace utils
{

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
    using AttachType = std::decay_t<__Type>;
    BaseSingleAttachWrapper<AttachType>::attach(std::forward<__Type>(attachment));
}

};