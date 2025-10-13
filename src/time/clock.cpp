#include "utils/time/clock.hpp"

namespace Utils::Time
{
    Clock::Clock()
        : m_start { SteadyClock::now() }
    { }

    void Clock::Start()
    {
        m_start = SteadyClock::now();
    }

    int64_t Clock::GetSeconds()
    {
        return std::chrono::duration_cast<Seconds>(SteadyClock::now() - m_start).count();
    }
    
    int64_t Clock::GetMilliseconds()
    {
        return std::chrono::duration_cast<Millisecs>(SteadyClock::now() - m_start).count();
    }
    
    int64_t Clock::GetMicroseconds()
    {
        return std::chrono::duration_cast<Microsecs>(SteadyClock::now() - m_start).count();
    }
    
    int64_t Clock::GetNanoseconds()
    {
        return std::chrono::duration_cast<Nanosecs>(SteadyClock::now() - m_start).count();
    }
}
