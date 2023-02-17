#pragma once

#include "cvk/vk_header.h"
#include "template_sequence.h"

#include <vector>
#include <string>

namespace utils
{

    template<typename _Ty>
    using RemoveCVREFType = std::remove_cv_t<std::remove_reference_t<_Ty>>;


    template<typename _Element>
    struct _TupleVal
    {
        constexpr _TupleVal() = default;

        template<typename __Other>
        constexpr _TupleVal(__Other&& other) : _val(std::forward<__Other>(other)) {}

        _Element _val;
    };
    
    template<typename..._Args>
    struct _TupleImpl;


    template<typename _Type, typename..._Args>
    struct _TupleImpl<_Type, _Args...> : 
        public _TupleImpl<_Args...>
    {
        using _FirstType = _Type;
        using _MyType = _TupleImpl<_Type, _Args...>;
        using _BaseType = _TupleImpl<_Args...>;
        static constexpr size_t size = 1 + sizeof...(_Args);

        constexpr _TupleImpl() = default;

        template<typename __Head, typename...__Tail>
        constexpr _TupleImpl(__Head&& head, __Tail&&...tail) :
            _BaseType(std::forward<__Tail>(tail)...), _first(std::forward<__Head>(head)) {}
        
        constexpr _TupleImpl(_TupleImpl CONST_REFERENCE) = default;

        inline constexpr auto get_data() -> _FirstType&;
        inline constexpr auto get_data() const -> _FirstType CONST_REFERENCE;

        _TupleVal<_Type> _first;

    };
    
    template<>
    struct _TupleImpl<> 
    {
        constexpr _TupleImpl() noexcept = default;
    };


    template<typename..._Elements>
    struct Tuple : public _TupleImpl<_Elements...>
    {
        
        using TupleType = _TupleImpl<_Elements...>;

        constexpr Tuple() = default;
        constexpr Tuple(_Elements CONST_REFERENCE...elements) : TupleType(elements...) {}
        constexpr Tuple(Tuple CONST_REFERENCE) = default;
        // constexpr auto operator=(Tuple CONST_REFERENCE) -> Tuple& = default;
        
        inline constexpr auto to_std() const -> std::tuple<_Elements...>;
    };
    
    template<>
    struct Tuple<>
    {
        constexpr Tuple() = default;
    };


    template<typename>
    struct MakeSequenceByTupleSizeImpl;
    template<
        template<typename...> typename _TupleType,
        typename..._TArgs>
    struct MakeSequenceByTupleSizeImpl<_TupleType<_TArgs...>>
    {
        using sequence = MakeIndexSequenceBySize<sizeof...(_TArgs)>;
    };
    template<typename _Ty>
    using MakeSequenceByTupleSize = typename MakeSequenceByTupleSizeImpl<_Ty>::sequence;


    template<size_t, typename>
    struct TupleElement;

    template<size_t _I, 
        template<typename...> typename _TupleType,
        typename _First, typename..._Rest>
    struct TupleElement<_I, _TupleType<_First, _Rest...>> : public TupleElement<_I - 1, _TupleType<_Rest...>>
    {};
    
    template<
        template<typename...> typename _TupleType,
        typename _First, typename..._Rest>
    struct TupleElement<0, _TupleType<_First, _Rest...>>
    {
        using type = _First;
        using _Ttype = _TupleType<_First, _Rest...>;
    };
    
    template<size_t _I, typename _T>
    using TupleElementType = typename TupleElement<_I, RemoveCVREFType<_T>>::type;

    template<size_t __I, typename...__TupleArgs, typename __TupleType = typename Tuple<__TupleArgs...>::TupleType>
    inline constexpr auto get(Tuple<__TupleArgs...>& tuple) -> TupleElementType<__I, __TupleType>&
    {
        using _type = typename TupleElement<__I, __TupleType>::_Ttype;
        return static_cast<_type&>(tuple).get_data();
    }
    
    template<size_t __I, typename...__TupleArgs, typename __TupleType = typename Tuple<__TupleArgs...>::TupleType>
    inline constexpr auto get(Tuple<__TupleArgs...> CONST_REFERENCE tuple) -> TupleElementType<__I, __TupleType> CONST_REFERENCE
    {
        using _type = typename TupleElement<__I, __TupleType>::_Ttype;
        return static_cast<_type CONST_REFERENCE>(tuple).get_data();
    }

    template<template<typename...> typename, typename, typename, typename, size_t, typename...>
    struct _TupleCatImpl;

    template<
        template<typename...> typename _TupleType, 
        typename _Ty, size_t..._RLi, size_t..._RTi, size_t _Next, size_t..._Ti, typename..._Rest>
    struct _TupleCatImpl<_TupleType, _Ty, IndexSequence<_RLi...>, IndexSequence<_RTi...>, _Next, IndexSequence<_Ti...>, _Rest...> :
        public _TupleCatImpl<
            _TupleType, _Ty, 
            IndexSequence<_RLi..., (_Next + 0 * _Ti)...>, IndexSequence<_RTi..., _Ti...>, 
            _Next + 1, _Rest...>
    {};

    template<
        template<typename...> typename _TupleType, 
        typename _Ty, size_t..._RLi, size_t..._RTi, size_t _Next>
    struct _TupleCatImpl<_TupleType, _Ty, IndexSequence<_RLi...>, IndexSequence<_RTi...>, _Next>
    {
        using type = _TupleType<TupleElementType<_RTi, TupleElementType<_RLi, _Ty>>...>;
        using list_index = IndexSequence<_RLi...>;
        using tuple_index = IndexSequence<_RTi...>;
    };

    template<
        template<typename...> typename _TupleType, 
        typename..._Tuples>
    using TupleCatImpl = _TupleCatImpl<
        _TupleType, _TupleType<_Tuples...>, 
        IndexSequence<>, IndexSequence<>, 0, 
        MakeSequenceByTupleSize<RemoveCVREFType<_Tuples>>...>;
    
    template<typename..._Tuples>
    using TupleCat = TupleCatImpl<Tuple, _Tuples...>;
    
    template<typename..._Tuples>
    using TupleCatType = typename TupleCat<_Tuples...>::type;

    template<typename _RetType, size_t..._Li, size_t..._Ti, typename _Tuple>
    inline constexpr auto _tuple_cat_fill(IndexSequence<_Li...>, IndexSequence<_Ti...>, _Tuple&& tuple) ->
        _RetType
    {
        return _RetType(get<_Ti>(get<_Li>(std::forward<_Tuple>(tuple)))...);
    }

    template<typename..._Tuples>
    inline constexpr auto tuple_cat(_Tuples&&...tuples) ->
        TupleCatType<_Tuples...>
    {
        using _RetTupleInfo = TupleCat<_Tuples...>;
        using RetType = typename _RetTupleInfo::type;
        using li = typename _RetTupleInfo::list_index;
        using ti = typename _RetTupleInfo::tuple_index;
        return _tuple_cat_fill<RetType>(li(), ti(), Tuple<_Tuples...>(std::forward<_Tuples>(tuples)...));
    }


    template<typename>
    struct _MakeTupleFromSequenceImpl;

    template<typename _ElementType, _ElementType..._Ti>
    struct _MakeTupleFromSequenceImpl<Sequence<_ElementType, _Ti...>>
    {
        static constexpr Tuple tuple = { _Ti... };
    };

    
    template<typename _Sequence>
    static constexpr Tuple MakeTupleFromSequence = _MakeTupleFromSequenceImpl<_Sequence>::tuple;


} // namespace utils

#include "tuple.inl"