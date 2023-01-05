#pragma once

namespace utils
{

static void __wrapper(...) 
{
}

// template<class _Type>
// template<class __Type>
// constexpr BaseMultipleAttachWrapper<_Type>::BaseMultipleAttachWrapper(std::initializer_list<__Type> CONST_REFERENCE list)
// {
//     _attachments.insert(_attachments.end(), list.begin(), list.end());
// }

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
BaseMultipleAttachWrapper<_Type>::operator std::vector<_Type> CONST_REFERENCE () const
{
    return _attachments;
}

template<class... _Type>
template<class __Type>
void BaseMultipleAttaches<_Type...>::attaches(__Type&& obj)
{
    this->BaseMultipleAttachWrapper<decltype(remove_clr<__Type>())>::_attach(obj);
}

template<class... _Type>
template<class __Type, class...__Args>
void BaseMultipleAttaches<_Type...>::attaches(__Type&& obj, __Args&&...args)
{
    
    this->BaseMultipleAttachWrapper<decltype(remove_clr<__Type>())>::_attach(obj);
    attaches(std::forward<__Args>(args)...);
}

template<class... _Type>
void BaseMultipleAttaches<_Type...>::clear_all()
{
    // __wrapper(BaseMultipleAttachWrapper<_Type>::_clear()...);
    _clear_recursive(static_cast<BaseMultipleAttachWrapper<_Type>&>(*this)...);
}

template<class... _Type>
template<class __Type>
void BaseMultipleAttaches<_Type...>::_clear_recursive(__Type&& obj)
{
    obj.clear();
}

template<class... _Type>
template<class __Type, class...__Args>
void BaseMultipleAttaches<_Type...>::_clear_recursive(__Type&& obj, __Args&&...args)
{
    obj.clear();
    // BaseMultipleAttachWrapper<__Type>::_clear();
    _clear_recursive(std::forward<__Args>(args)...);
}

};
