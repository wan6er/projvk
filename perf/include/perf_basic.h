#pragma once

#if defined(WIN32)

extern "C" __declspec(dllimport) void DebugBreak();

#define CPERF_PRINT_FILE_LINE(msg, ...) printf("%s:%d error %s :" msg "\n", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define CPERF_ASSERT(expression) do { if (!(expression)) { CPERF_PRINT_FILE_LINE("%s", #expression); DebugBreak(); } } while (false)

#if defined(CPERF_BUILD)
    #define CPERF_API __declspec(dllexport)
#else
    #define CPERF_API __declspec(dllimport)
#endif

#elif defined(linux)

#define CPERF_PRINT_FILE_LINE(msg, ...) printf("%s:%d error %s :" msg "\n", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define CPERF_ASSERT(expression) do { if (!(expression)) { CPERF_PRINT_FILE_LINE("%s", #expression); __builtin_trap(); } } while (false)

#if defined(CPERF_BUILD)
    #define CPERF_API
#else
    #define CPERF_API
#endif

#endif
