#pragma once

#include "cvk/vk_header.h"

#include <cstdint>
#include <chrono>

namespace utils
{

class BaseTimer
{
public:
    virtual ~BaseTimer() = default;
    
    virtual void start() = 0;
    virtual uint64_t stop() = 0;
};

class CVK_API Timer : public BaseTimer
{
public:
    virtual void start();
    virtual uint64_t stop();

private:
    std::chrono::system_clock::time_point _tp;

};

class CVK_API Stopwatch : public Timer
{
public:
    Stopwatch();

    virtual uint64_t lap();

};

} // namespace utils