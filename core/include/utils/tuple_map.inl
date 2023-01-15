#pragma once

namespace utils
{
    
    template<class _Type>
    constexpr ObjectDescriptor<_Type>::ObjectDescriptor(std::string CONST_REFERENCE name) noexcept :
        _name(name)
    {
    }
    
    template<class _Type>
    constexpr std::string CONST_REFERENCE ObjectDescriptor<_Type>::get_name() const
    {
        return _name;
    }
    
    template<class _Object, class..._Args>
    template<class...__Args>
    constexpr ObjectTuple<_Object, _Args...>::ObjectTuple(std::string CONST_REFERENCE name, __Args&&...args) noexcept :
        ObjectTuple<_Args...>(std::forward<__Args>(args)...),
        _desc(name)
    {
    }

    template<class _Object, class..._Args>
    constexpr auto ObjectTuple<_Object, _Args...>::get_descriptor(std::string CONST_REFERENCE name)
    {
        if constexpr (_desc.get_name() == name) {
            return _desc;
        } else {
            return ObjectTuple<_Args...>::get_descriptor(name);
        }
    }

    template<class _Object>
    constexpr ObjectTuple<_Object>::ObjectTuple(std::string CONST_REFERENCE name) noexcept :
        _desc(name)
    {
    }
    
    template<class _Object>
    constexpr auto ObjectTuple<_Object>::get_descriptor(std::string CONST_REFERENCE name)
    {
        if constexpr (_desc.get_name() == name) {
            return _desc;
        }
        throw std::exception();
    }

} // namespace utils
