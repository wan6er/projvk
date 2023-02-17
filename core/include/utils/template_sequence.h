#ifndef __TEMPLATE_SEQUENCE_H__
#define __TEMPLATE_SEQUENCE_H__

namespace utils
{
    


    template<typename _SeqType, _SeqType..._Rest>
    struct _SequenceImpl
    {
        using type = _SeqType;
        static constexpr _SeqType size = sizeof...(_Rest);
    };
        
    template<typename _SeqType, _SeqType..._Rest>
    using Sequence = _SequenceImpl<_SeqType, _Rest...>;

    template<size_t..._Rest>
    using IndexSequence = Sequence<size_t, _Rest...>;

    template<size_t...>
    struct MakeIndexSequenceImpl;

    template<size_t _Start, size_t _End, size_t..._Rest>
    struct MakeIndexSequenceImpl<_Start, _End, _Rest...>
    {
        using sequence = typename MakeIndexSequenceImpl<_Start, _End - 1, _End - 1, _Rest...>::sequence;
    };
    template<size_t _Start, size_t..._Rest>
    struct MakeIndexSequenceImpl<_Start, _Start, _Rest...>
    {
        using sequence = utils::IndexSequence<_Rest...>;
    };
    
    template<size_t _Start, size_t _End>
    using MakeIndexSequence = typename MakeIndexSequenceImpl<_Start, _End>::sequence;
    
    template<size_t _Size>
    using MakeIndexSequenceBySize = MakeIndexSequence<0, _Size>;


    template<typename, typename>
    struct _SequenceReverseImpl;

    template<typename _ElementType, _ElementType _LFirst, _ElementType..._LTail, _ElementType..._RElements>
    struct _SequenceReverseImpl<Sequence<_ElementType, _LFirst, _LTail...>, Sequence<_ElementType, _RElements...>> :
        public _SequenceReverseImpl<Sequence<_ElementType, _LTail...>, Sequence<_ElementType, _LFirst, _RElements...>>
    {
    };
    
    template<typename _ElementType, _ElementType..._RElements>
    struct _SequenceReverseImpl<Sequence<_ElementType>, Sequence<_ElementType, _RElements...>>
    {
        using sequence = Sequence<_ElementType, _RElements...>;
    };
    
    template<typename _Sequence>
    using SequenceReverse = typename _SequenceReverseImpl<_Sequence, Sequence<typename _Sequence::type>>::sequence;


    
    template<typename _Ty, _Ty, typename>
    struct _AppendSequenceImpl;

    template<typename _Ty, _Ty _AppendElement, _Ty..._Tail>
    struct _AppendSequenceImpl<_Ty, _AppendElement, Sequence<_Ty, _Tail...>>
    {
        using sequence = Sequence<_Ty, _Tail..., _AppendElement>;
    };

    template<typename _Ty, _Ty _Element, typename _Sequence>
    using AppendSequence = typename _AppendSequenceImpl<_Ty, _Element, _Sequence>::sequence;

} // namespace utils

#endif