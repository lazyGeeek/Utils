#pragma once

#include "utils/time/utils.hpp"

namespace Utils::Time
{
    class Timer
    {
    public:
        Timer();

        void Start();

        int64_t GetSeconds();
        int64_t GetMilliseconds();
        int64_t GetMicroseconds();

    private:
        SteadyClock::time_point m_start;
    };
} // namespace Utils::Time

