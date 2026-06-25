#pragma once

#include <ctime>
#include <string>

namespace Utils::Time
{
    struct Time
    {
        std::tm Time;
        int64_t Milliseconds;
        int64_t Microseconds;
        int64_t Nanoseconds;
    };

    Time GetNow();

    std::string GetDate();
    std::string GetTime();
    std::string GetDateAndTime();
} // namespace Utils::Time

