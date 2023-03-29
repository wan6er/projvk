#pragma once

#if defined(WIN32)

extern "C" __declspec(dllimport) void DebugBreak();

#include <stdio.h>
#define CPERF_PRINT_FILE_LINE(msg, ...) printf("%s:%d error %s :" msg "\n", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define CLF_ASSERT(expression) do { if (!(expression)) { CPERF_PRINT_FILE_LINE("%s", #expression); DebugBreak(); } } while (false)

#if defined(CLF_BUILD)
    #define CLF_API __declspec(dllexport)
#else
    #define CLF_API __declspec(dllimport)
#endif

#else

#include <stdio.h>

#define CLF_PRINT_FILE_LINE(msg, ...) printf("%s:%d error %s :" msg "\n", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define CLF_ASSERT(expression) do { if (!(expression)) { CLF_PRINT_FILE_LINE("%s", #expression); __builtin_trap(); } } while (false)

#if defined(CLF_BUILD)
    #define CLF_API
#else
    #define CLF_API
#endif

#endif
