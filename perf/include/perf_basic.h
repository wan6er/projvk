#pragma once

#if defined(CPERF_BUILD)
    #define CPERF_API __declspec(dllexport)
#else
    #define CPERF_API __declspec(dllimport)
#endif