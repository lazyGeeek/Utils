#pragma once
#include "utils/non_copyable.hpp"
#include "utils/services/service_interface.hpp"
#include "utils/time/utils.hpp"

namespace Utils::Time
{
    class GlobalClock : public NonCopyable, public Services::IService
    {
    public:
        GlobalClock();
        ~GlobalClock() override = default;

        void Start();
        void UpdateDelta();

        double GetDeltaMillisec();
        double GetDurationMillisec();

    private:
        SteadyClock::time_point m_start;
        SteadyClock::time_point m_lastDeltaPoint;

        Duration m_delta { 0.0 };
    };
} // namespace Utils::Time

