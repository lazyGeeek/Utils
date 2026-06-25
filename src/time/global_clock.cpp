#include "utils/time/global_clock.hpp"

namespace Utils::Time
{
    GlobalClock::GlobalClock()
        : m_start { SteadyClock::now() },
          m_lastDeltaPoint { SteadyClock::now() }
    { }

    void GlobalClock::Start()
    {
        m_start = SteadyClock::now();
    }

    void GlobalClock::UpdateDelta()
    {
        SteadyClock::time_point now = SteadyClock::now();
        m_delta = now - m_lastDeltaPoint;
        m_lastDeltaPoint = now;
    }

    double GlobalClock::GetDeltaMillisec()
    {
        return DurationCast<Millisecs>(m_delta).count();
    }

    double GlobalClock::GetDurationMillisec()
    {
        return DurationCast<Millisecs>(SteadyClock::now() - m_start).count();
    }
} // namespace Utils::Time

