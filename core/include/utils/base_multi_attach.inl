#pragma once

#include <type_traits>
#include <utility>

namespace utils
{

template<class _Type>
void BaseMultipleAttachWrapper<_Type>::_attach(_Type CONST_REFERENCE attachment) 
{
    _attachments.push_back(attachment);
}

template<class _Type>
void BaseMultipleAttachWrapper<_Type>::clear()
{
    _attachments.clear();
}

template<class _Type>
auto BaseMultipleAttachWrapper<_Type>::get_attachments() -> std::vector<_Type>&
{
    return _attachments;
}

template<class _Type>
auto BaseMultipleAttachWrapper<_Type>::get_attachments() const -> std::vector<_Type> CONST_REFERENCE
{
    return _attachments;
}
        
template<class _Type>
BaseMultipleAttachWrapper<_Type>::operator std::vector<_Type> CONST_REFERENCE () const
{
    return _attachments;
}

template<class _Type>
BaseMultipleAttachWrapper<_Type>::operator std::vector<_Type>& ()
{
    return _attachments;
}


template<class... _Type>
template<class __Type>
void BaseMultipleAttaches<_Type...>::attaches(__Type&& obj)
{
    using AttachType = std::decay_t<__Type>;
    this->BaseMultipleAttachWrapper<AttachType>::_attach(std::forward<__Type>(obj));
}

template<class... _Type>
template<class __Type, class...__Args>
void BaseMultipleAttaches<_Type...>::attaches(__Type&& obj, __Args&&...args)
{
    attaches(std::forward<__Type>(obj));
    (attaches(std::forward<__Args>(args)), ...);
}

template<class... _Type>
void BaseMultipleAttaches<_Type...>::clear_all()
{
    (static_cast<BaseMultipleAttachWrapper<_Type>&>(*this).clear(), ...);
}

};
