#pragma once

#define CVK_DEF_EXPAND(...) __VA_ARGS__

#define CVK_DEF_COUNT_ARGS_( \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, \
    _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, \
    _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, \
    _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, n, ...) n

#define CVK_DEF_COUNT_ARGS(...) CVK_DEF_EXPAND(CVK_DEF_COUNT_ARGS_(__VA_ARGS__, \
        63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, \
        48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, \
        32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, \
        16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))

#define CVK_DEF_CONCAT_(l, r) l ## r
#define CVK_DEF_CONCAT(l, r) CVK_DEF_CONCAT_(l, r)

#define CVK_DEF_FOREACH_PAIR0(separator, userdata, func) 
#define CVK_DEF_FOREACH_PAIR1(separator, userdata, func, arg) func(userdata, arg) separator
#define CVK_DEF_FOREACH_PAIR2(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR1(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR3(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR2(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR4(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR3(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR5(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR4(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR6(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR5(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR7(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR6(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR8(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR7(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR9(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR8(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR10(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR9(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR11(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR10(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR12(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR11(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR13(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR12(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR14(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR13(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR15(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR14(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR16(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR15(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR17(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR16(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR18(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR17(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR19(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR18(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR20(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR19(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR21(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR20(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR22(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR21(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR23(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR22(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR24(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR23(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR25(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR24(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR26(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR25(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR27(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR26(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR28(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR27(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR29(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR28(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR30(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR29(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR31(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR30(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR32(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR31(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR33(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR32(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR34(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR33(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR35(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR34(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR36(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR35(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR37(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR36(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR38(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR37(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR39(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR38(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR40(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR39(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR41(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR40(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR42(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR41(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR43(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR42(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR44(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR43(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR45(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR44(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR46(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR45(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR47(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR46(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR48(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR47(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR49(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR48(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR50(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR49(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR51(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR50(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR52(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR51(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR53(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR52(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR54(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR53(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR55(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR54(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR56(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR55(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR57(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR56(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR58(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR57(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR59(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR58(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR60(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR59(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR61(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR60(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR62(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR61(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR63(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR62(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR64(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR63(separator, userdata, func, __VA_ARGS__))
#define CVK_DEF_FOREACH_PAIR65(separator, userdata, func, arg, ...) func(userdata, arg) separator CVK_DEF_EXPAND(CVK_DEF_FOREACH_PAIR64(separator, userdata, func, __VA_ARGS__))

#define CVK_DEF_FOREACH(separator, userdata, func, ...) CVK_DEF_EXPAND(CVK_DEF_CONCAT(CVK_DEF_FOREACH_PAIR, CVK_DEF_COUNT_ARGS(__VA_ARGS__))(separator, userdata, func, __VA_ARGS__))
    