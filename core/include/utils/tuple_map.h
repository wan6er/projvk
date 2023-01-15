#pragma once

#include "cvk/vk_header.h"

#include <string>

namespace utils
{
    
    template<class _Type>
    class ObjectDescriptor
    {
    public:
        using ObjectType = _Type;
        
        constexpr ObjectDescriptor(std::string CONST_REFERENCE name) noexcept;
        virtual ~ObjectDescriptor() = default;

        constexpr std::string CONST_REFERENCE get_name() const;

    private:
        const std::string _name;
    };

    template<class..._Args>
    class ObjectTuple;

    template<class _Object, class..._Args>
    class ObjectTuple<_Object, _Args...> : public ObjectTuple<_Args...>
    {
    public:
        template<class...__Args>
        constexpr ObjectTuple(std::string CONST_REFERENCE name, __Args&&... args) noexcept;
        virtual ~ObjectTuple() = default;

        constexpr auto get_descriptor(std::string CONST_REFERENCE name);

    private:
        ObjectDescriptor<_Object> _desc;
    };

    template<class _Object>
    class ObjectTuple<_Object>
    {
    public:
        constexpr ObjectTuple(std::string CONST_REFERENCE name) noexcept;
        virtual ~ObjectTuple() = default;

        constexpr auto get_descriptor(std::string CONST_REFERENCE name);
        
    private:
        ObjectDescriptor<_Object> _desc;
    };

} // namespace utils

#include "tuple_map.inl"