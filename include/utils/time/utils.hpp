#pragma once

#include <chrono>

namespace Utils::Time
{
    using Seconds = std::chrono::seconds;
    using Millisecs = std::chrono::milliseconds;
    using Microsecs = std::chrono::microseconds;
    using Nanosecs = std::chrono::nanoseconds;

    using SystemClock = std::chrono::system_clock;
    using SteadyClock = std::chrono::steady_clock;

    using Duration = std::chrono::duration<double>;

    template <class ToDuration>
        requires(std::is_same_v<ToDuration, std::chrono::seconds> ||
                 std::is_same_v<ToDuration, std::chrono::milliseconds> ||
                 std::is_same_v<ToDuration, std::chrono::microseconds> ||
                 std::is_same_v<ToDuration, std::chrono::nanoseconds>)
    auto DurationCast(const Duration& duration)
    {
        return std::chrono::duration_cast<ToDuration>(duration);
    }
} // namespace Utils::Time

