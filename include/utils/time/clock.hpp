#pragma once
#ifndef UTILS_TIME_CLOCK_HPP_
#define UTILS_TIME_CLOCK_HPP_

#include <chrono>

namespace Utils::Time
{
    class Clock
    {
        using Seconds = std::chrono::seconds;
        using Millisecs = std::chrono::milliseconds;
        using Microsecs = std::chrono::microseconds;
        using Nanosecs = std::chrono::nanoseconds;
        using SteadyClock = std::chrono::steady_clock;

    public:
        Clock();

        void Start();

        int64_t GetSeconds();
        int64_t GetMilliseconds();
        int64_t GetMicroseconds();
        int64_t GetNanoseconds();

    private:
        SteadyClock::time_point m_start;
    };
}

#endif // UTILS_TIME_CLOCK_HPP_
