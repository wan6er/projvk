/*

    sorting base on meta-programming

    useless, just for funny.

*/
#pragma once

#include "tuple.h"
#include "template_reverse.h"

namespace utils
{
    
    template<typename _ValType, bool, _ValType, _ValType>
    struct _IfValImpl;
    
    template<typename _ValType, _ValType _TrueVal, _ValType _FalseVal>
    struct _IfValImpl<_ValType, true, _TrueVal, _FalseVal>
    {
        static constexpr _ValType val = _TrueVal;
    };
    
    template<typename _ValType, _ValType _TrueVal, _ValType _FalseVal>
    struct _IfValImpl<_ValType, false, _TrueVal, _FalseVal>
    {
        static constexpr _ValType val = _FalseVal;
    };
    
    template<typename _ValType, bool _Condition, _ValType _TrueVal, _ValType _FalseVal>
    static constexpr _ValType IfVal = _IfValImpl<_ValType, _Condition, _TrueVal, _FalseVal>::val;

    template<bool _Condition, int _TrueVal, int _FalseVal>
    static constexpr int IntegerIfVal = IfVal<int, _Condition, _TrueVal, _FalseVal>;
    template<bool _Condition, int _TrueVal, int _FalseVal>
    static constexpr int IndexIfVal = IfVal<size_t, _Condition, _TrueVal, _FalseVal>;



    template<typename _ValType, _ValType _First, _ValType _Second>
    struct _GreaterThanImpl
    {
        static constexpr bool val = _First > _Second;
    };

    template<typename _ValType, _ValType _First, _ValType _Second>
    static constexpr bool GreaterThanVal = _GreaterThanImpl<_ValType, _First, _Second>::val;

    
    template<typename _ValType, _ValType _First, _ValType _Second>
    struct _EqualImpl
    {
        static constexpr bool val = _First == _Second;
    };

    template<typename _ValType, _ValType _First, _ValType _Second>
    static constexpr bool EqualVal = _EqualImpl<_ValType, _First, _Second>::val;

    template<int _First, int _Second>
    static constexpr bool IntegerEqualVal = EqualVal<int, _First, _Second>;
    template<size_t _First, size_t _Second>
    static constexpr bool IndexEqualVal = EqualVal<size_t, _First, _Second>;


    template<typename _ValType, size_t, size_t, _ValType..._Elements>
    struct _Max;

    template<typename _ValType, size_t _Next, size_t _Index, _ValType _First, _ValType _Second, _ValType..._Elements>
    struct _Max<_ValType, _Next, _Index, _First, _Second, _Elements...> :
        _Max<_ValType, _Next + 1, 
            IfVal<size_t, GreaterThanVal<_ValType, _First, _Second>, _Index, _Next + 1>,
            IfVal<_ValType, GreaterThanVal<_ValType, _First, _Second>, _First, _Second>, _Elements...>
    {
    };
    
    template<typename _ValType, size_t _Next, size_t _Index, _ValType _First>
    struct _Max<_ValType, _Next, _Index, _First>
    {
        static constexpr size_t index = _Index;
        static constexpr _ValType max = _First;
    };

    template<typename _ValType, _ValType..._Elements>
    using Max = _Max<_ValType, 0, 0, _Elements...>;


    template<size_t _Start, size_t _Next, typename _ValType, _ValType..._Elements>
    struct _RangeMaxImpl;

    template<size_t _Start, size_t _Next, typename _ValType, _ValType _Head, _ValType..._Tail>
    struct _RangeMaxImpl<_Start, _Next, _ValType, _Head, _Tail...> :
        public _RangeMaxImpl<_Start, _Next - 1, _ValType, _Tail...>
    {};
    
    template<size_t _Start, typename _ValType, _ValType..._Elemenst>
    struct _RangeMaxImpl<_Start, 0, _ValType, _Elemenst...>
    {
        using _Max = Max<_ValType, _Elemenst...>;
        static constexpr size_t index = _Start + _Max::index;
        static constexpr _ValType max = _Max::max;
    };
    template<size_t _Start, typename _ValType, _ValType..._Elemenst>
    using RangeMax = _RangeMaxImpl<_Start, _Start, _ValType, _Elemenst...>;

    template<size_t, typename>
    struct _MaxSequnceDeduce;

    template<size_t _Start, typename _SeqType, _SeqType... _SeqRest>
    struct _MaxSequnceDeduce<_Start, Sequence<_SeqType, _SeqRest...>>
    {
        using _Max = RangeMax<_Start, _SeqType, _SeqRest...>;
        static constexpr size_t index = _Max::index;
        static constexpr _SeqType max = _Max::max;
    };

    template<size_t _Start, typename _Sequence>
    using RangeMaxSequence = _MaxSequnceDeduce<_Start, _Sequence>;

    template<typename _Sequence>
    using MaxSequence = _MaxSequnceDeduce<0, _Sequence>;


    
    template<size_t _Index, typename _SeqType, _SeqType...>
    struct _SequenceGetImpl;
    
    template<size_t _Index, typename _SeqType, _SeqType _Head, _SeqType..._Tail>
    struct _SequenceGetImpl<_Index, _SeqType, _Head, _Tail...> :
        public _SequenceGetImpl<_Index - 1, _SeqType, _Tail...>
    {
    };
        
    template<typename _SeqType, _SeqType _Head, _SeqType..._Tail>
    struct _SequenceGetImpl<0, _SeqType, _Head, _Tail...>
    {
        static constexpr _SeqType val = _Head;
    };

    template<size_t, typename>
    struct SequenceGet;
    
    template<size_t _Index, typename _SeqType, _SeqType..._Elements>
    struct SequenceGet<_Index, Sequence<_SeqType, _Elements...>>
    {
        static constexpr _SeqType val = _SequenceGetImpl<_Index, _SeqType, _Elements...>::val;
    };


    template<size_t...>
    struct _MakeSwapIndexSequenceImpl;
    
    template<size_t _FirstIndex, size_t _SecondIndex, size_t _Next, size_t..._Tail>
    struct _MakeSwapIndexSequenceImpl<_FirstIndex, _SecondIndex, _Next, _Tail...>
    {
        using sequence = typename _MakeSwapIndexSequenceImpl<_FirstIndex, _SecondIndex, _Next - 1, 
            IndexIfVal<IndexEqualVal<_Next - 1, _FirstIndex>, _SecondIndex, 
                IndexIfVal<IndexEqualVal<_Next - 1, _SecondIndex>, _FirstIndex, _Next - 1>>, 
            _Tail...>::sequence;
    };
    
    template<size_t _FirstIndex, size_t _SecondIndex, size_t..._Tail>
    struct _MakeSwapIndexSequenceImpl<_FirstIndex, _SecondIndex, 0, _Tail...>
    {
        using sequence = IndexSequence<_Tail...>;
    };

    template<size_t _FirstIndex, size_t _SecondIndex, size_t _Size>
    using MakeSwapIndexSequenceBySize = typename _MakeSwapIndexSequenceImpl<_FirstIndex, _SecondIndex, _Size>::sequence;



    template<typename, typename>
    struct _SwapSequenceIndexImpl;
    template<typename _SeqType, size_t..._Indices>
    struct _SwapSequenceIndexImpl<_SeqType, IndexSequence<_Indices...>>
    {
        using sequence = Sequence<typename _SeqType::type, SequenceGet<_Indices, _SeqType>::val...>;
    };

    template<size_t _First, size_t _Second, typename _SeqType>
    struct _SwapSequenceImpl
    {
        using sequence = typename _SwapSequenceIndexImpl<_SeqType, MakeSwapIndexSequenceBySize<_First, _Second, _SeqType::size>>::sequence;
    };

    template<size_t _First, size_t _Second, typename _SeqType>
    using SwapSequence = typename _SwapSequenceImpl<_First, _Second, _SeqType>::sequence;



    template<size_t _End, size_t _Next, typename _Sequence>
    struct _InsertionSortImpl
    : public
        _InsertionSortImpl<_End, _Next + 1, SwapSequence<_Next, RangeMaxSequence<_Next, _Sequence>::index, _Sequence>>
    {
        using _MaxSeq = RangeMaxSequence<_Next, _Sequence>;
        using _SwapedSeq = SwapSequence<_Next, _MaxSeq::index, _Sequence>;

        using _CurSequence = _Sequence;
        using _Base = _InsertionSortImpl<_End, _Next + 1, _SwapedSeq>;
    };
    
    template<size_t _End, typename _Sequence>
    struct _InsertionSortImpl<_End, _End, _Sequence>
    {
        using _CurSequence = _Sequence;
        using sequence = _Sequence;
    };

    
    template<typename _Sequence>
    using InsertionSort = typename _InsertionSortImpl<_Sequence::size, 0, _Sequence>::sequence;

} // namespace utils
