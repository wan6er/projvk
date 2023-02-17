#ifndef __TEMPLATE_IF_H__
#define __TEMPLATE_IF_H__

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
    

    template<typename _ValType, _ValType _First, _ValType _Second>
    struct _GreaterThanImpl
    {
        static constexpr bool val = _First > _Second;
    };

    
    template<typename _ValType, _ValType _First, _ValType _Second>
    struct _EqualImpl
    {
        static constexpr bool val = _First == _Second;
    };


    template<typename _ValType, bool _Condition, _ValType _TrueVal, _ValType _FalseVal>
    static constexpr _ValType IfVal = _IfValImpl<_ValType, _Condition, _TrueVal, _FalseVal>::val;

    template<bool _Condition, int _TrueVal, int _FalseVal>
    static constexpr int IntegerIfVal = IfVal<int, _Condition, _TrueVal, _FalseVal>;
    template<bool _Condition, int _TrueVal, int _FalseVal>
    static constexpr int IndexIfVal = IfVal<size_t, _Condition, _TrueVal, _FalseVal>;

    template<typename _ValType, _ValType _First, _ValType _Second>
    static constexpr bool EqualVal = _EqualImpl<_ValType, _First, _Second>::val;

    template<typename _ValType, _ValType _First, _ValType _Second>
    static constexpr bool GreaterThanVal = _GreaterThanImpl<_ValType, _First, _Second>::val;

    template<int _First, int _Second>
    static constexpr bool IntegerEqualVal = EqualVal<int, _First, _Second>;
    template<size_t _First, size_t _Second>
    static constexpr bool IndexEqualVal = EqualVal<size_t, _First, _Second>;

} // namespace utils


#endif