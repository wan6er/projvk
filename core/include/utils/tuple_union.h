#pragma once

#include "tuple.h"
#include "template_reverse.h"

namespace utils
{
    
    struct _TupleUnion_IndexType
    {
    };

    template<typename _Type, typename..._Elements>
    class TupleUnion
    {
    public:
        using TupleType = TemplatedReverseType<Tuple<_Elements...>>;
        using _TupleImplType = typename TemplatedReverseType<Tuple<_Elements...>>::TupleType;

        static constexpr size_t size = TupleType::size;
        
        union 
        {
            TupleType tuple;
            _Type data;
        };

        constexpr TupleUnion() = default;

        template<typename __Tag, size_t...__Ti, typename __This, std::enable_if_t<std::is_same_v<__Tag, _TupleUnion_IndexType>, int> = 0>
        constexpr TupleUnion(__Tag, IndexSequence<__Ti...>, __This CONST_REFERENCE args) :
            tuple(get<__Ti>(args)...) {}

        constexpr TupleUnion(_Elements CONST_REFERENCE...args) :
            TupleUnion(_TupleUnion_IndexType{}, MakeIndexSequenceBySize<sizeof...(args)>{}, reverse_parameter(args...)) {}
            
        constexpr TupleUnion(TupleUnion CONST_REFERENCE obj) : tuple(obj.tuple) {}

        inline constexpr auto operator->() -> _Type*;
        inline constexpr auto operator->() const -> _Type CONST_PTR;

    };

    template<size_t __i, typename..._Args>
    inline constexpr auto get(TupleUnion<_Args...>& u)
    {
        return get<TupleUnion<_Args...>::size - 1 - __i>(u.tuple);
    }

    template<size_t __i, typename..._Args>
    inline constexpr auto get(TupleUnion<_Args...> CONST_REFERENCE u)
    {        
        return get<TupleUnion<_Args...>::size - 1 - __i>(u.tuple);
    }

} // namespace utils

#include "tuple_union.inl"