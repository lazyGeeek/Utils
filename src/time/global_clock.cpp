#include "utils/time/global_clock.hpp"

namespace Utils::Time
{
    GlobalClock::GlobalClock()
        : m_start { SteadyClock::now() },
          m_lastDeltaPoint{ SteadyClock::now() },
          m_delta{ std::chrono::duration<double>(0.0) }
    { }

    void GlobalClock::Start()
    {
        m_start = SteadyClock::now();
    }

    void GlobalClock:: UpdateDelta()
    {
        SteadyClock::time_point now = SteadyClock::now();
        m_delta = now - m_lastDeltaPoint;
        m_lastDeltaPoint = now;
    }

    double GlobalClock::GetDeltaMS()
    {
        return std::chrono::duration_cast<Microsecs>(m_delta).count() / 1000.0;
    }

    double GlobalClock::GetDurationMS()
    {
        return std::chrono::duration_cast<Microsecs>(SteadyClock::now() - m_start).count() / 1000.0;
    }
}
