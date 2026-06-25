#include "utils/time/timer.hpp"

namespace Utils::Time
{
    Timer::Timer() : m_start { SteadyClock::now() }
    { }

    void Timer::Start()
    {
        m_start = SteadyClock::now();
    }

    int64_t Timer::GetSeconds()
    {
        return DurationCast<Seconds>(SteadyClock::now() - m_start).count();
    }

    int64_t Timer::GetMilliseconds()
    {
        return DurationCast<Millisecs>(SteadyClock::now() - m_start).count();
    }

    int64_t Timer::GetMicroseconds()
    {
        return DurationCast<Microsecs>(SteadyClock::now() - m_start).count();
    }
} // namespace Utils::Time

