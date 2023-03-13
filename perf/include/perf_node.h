#ifndef __PERF_NODE_H__
#define __PERF_NODE_H__

#include "perf_basic.h"

#include <chrono>

namespace cperf
{

using Time = std::chrono::system_clock::time_point;

CPERF_API auto now() -> Time;

template<typename _RetTime, typename _Time>
auto time_cast(_Time const& time_point) -> uint64_t {
    return std::chrono::duration_cast<_RetTime>(time_point).count();
}
    
struct CPERF_API PerfNode
{
    Time start;
    Time end;

    void set_start();
    void set_end();

    template<typename _Time>
    constexpr auto get_start() const -> Time {
        return start;
    }
    
    template<typename _Time>
    constexpr auto get_end() const -> Time {
        return start;
    }

};


} // namespace cperf

#endif