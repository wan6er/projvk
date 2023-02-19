#pragma once

#if defined(WIN32)

#if defined(CPERF_BUILD)
    #define CPERF_API __declspec(dllexport)
#else
    #define CPERF_API __declspec(dllimport)
#endif

#elif defined(linux)

#if defined(CPERF_BUILD)
    #define CPERF_API
#else
    #define CPERF_API
#endif

#endif