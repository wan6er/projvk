#pragma once

#include "tuple.h"

namespace utils
{
    

    template<typename>
    struct TemplatedBaseCase;

    template<template<typename...> class T, typename..._TArgs>
    struct TemplatedBaseCase<T<_TArgs...>>
    {
        using type = T<>;
    };
    
    template<typename _Ty>
    using TemplatedBaseCaseType = typename TemplatedBaseCase<_Ty>::type;

    template<
        typename _L,
        typename = typename TemplatedBaseCase<_L>::type>
    struct TemplatedReverseImpl;
    
    template<
        template<typename...> typename _R,
        typename..._RArgs>
    struct TemplatedReverseImpl<typename TemplatedBaseCase<_R<_RArgs...>>::type, _R<_RArgs...>>
    {
        using type = _R<_RArgs...>;
    };

    template<
        template<typename...> typename _L,
        template<typename...> typename _R,
        typename _First,
        typename..._Left,
        typename..._Right>
    struct TemplatedReverseImpl<_L<_First, _Left...>, _R<_Right...>>
    {
        using type = typename TemplatedReverseImpl<_L<_Left...>, _R<_First, _Right...>>::type;
    };

    template<typename..._Types>
    struct TemplatedList {};

    template<typename _L, typename _R>
    struct TemplatedSwap;

    template<
        template<typename...> typename _L,
        template<typename...> typename _R,
        typename..._LArgs,
        typename..._RArgs>
    struct TemplatedSwap<_L<_LArgs...>, _R<_RArgs...>>
    {
        using left = _L<_RArgs...>;
        using right = _R<_LArgs...>;
    };

    template<typename>
    struct TemplatedReverse;
    
    template<
        template<typename...> typename _Type,
        typename..._Args>
    struct TemplatedReverse<_Type<_Args...>>
    {
        using types = TemplatedList<_Args...>;
        using reversed_types = typename TemplatedReverseImpl<types>::type;
        using type = typename TemplatedSwap<_Type<_Args...>, reversed_types>::left;
    };

    template<typename _Type>
    using TemplatedReverseType = typename TemplatedReverse<_Type>::type;

    
    template<typename __RetType, typename __LFirst, typename...__LArgs>
    constexpr auto reverse_parameter_impl(Tuple<> CONST_REFERENCE largs, __LFirst&& first, __LArgs&&...args) -> 
        __RetType
    {
        return reverse_parameter_impl<__RetType>(Tuple<__LFirst>(std::forward<__LFirst>(first)), std::forward<__LArgs>(args)...);
    }

    template<typename __RetType, typename __LFirst, typename...__Args, typename...__LArgs,
        std::enable_if_t<(sizeof...(__Args) > 0), int> = 0>
    constexpr auto reverse_parameter_impl(Tuple<__Args...> CONST_REFERENCE largs, __LFirst&& first, __LArgs&&...args) -> 
        __RetType
    {
        return reverse_parameter_impl<__RetType>(
            tuple_cat(Tuple<__LFirst>(std::forward<__LFirst>(first)), largs),
            std::forward<__LArgs>(args)...);
    }

    template<typename __RetType, typename...__RetArgs>
    constexpr auto reverse_parameter_impl(Tuple<__RetArgs...> CONST_REFERENCE reversed_args) -> 
        __RetType
    {
        return reversed_args;
    }

    template<typename...__Args>
    constexpr auto reverse_parameter(__Args&&...args) ->
        TemplatedReverseType<Tuple<__Args...>>
    {
        return reverse_parameter_impl<TemplatedReverseType<Tuple<__Args...>>>(Tuple<>(), std::forward<__Args>(args)...);
    }

    

} // namespace utils
