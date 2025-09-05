#pragma once
#ifndef UTILS_TIME_GLOBAL_CLOCK_HPP_
#define UTILS_TIME_GLOBAL_CLOCK_HPP_

#include <chrono>

#include "utils/services/service_interface.hpp"

namespace Utils::Time
{
    class GlobalClock : public Services::IService
    {
        using Microsecs = std::chrono::microseconds;
        using SteadyClock = std::chrono::steady_clock;

    public:
        GlobalClock();
        virtual ~GlobalClock() = default;

        GlobalClock(const GlobalClock& other)             = delete;
        GlobalClock(GlobalClock&& other)                  = delete;
        GlobalClock& operator=(const GlobalClock& other)  = delete;
        GlobalClock& operator=(const GlobalClock&& other) = delete;

        void Start();
        void UpdateDelta();

        double GetDeltaMS();
        double GetDurationMS();

    private:
        SteadyClock::time_point m_start;
        SteadyClock::time_point m_lastDeltaPoint;

        std::chrono::duration<double> m_delta;
    };
}

#endif // UTILS_TIME_GLOBAL_CLOCK_HPP_
