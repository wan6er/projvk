#include "utils/timer.h"

namespace utils
{
    
void Timer::start()
{
    _tp = std::chrono::system_clock::now();
}

uint64_t Timer::stop()
{
    auto cur = std::chrono::system_clock::now();
    return static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(cur - _tp).count());
}

Stopwatch::Stopwatch()
{
    start();
}

uint64_t Stopwatch::lap()
{
    uint64_t time = stop();
    start();
    return time;
}

} // namespace utils
