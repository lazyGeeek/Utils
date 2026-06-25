#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "utils/time/global_clock.hpp"

using namespace std::chrono_literals;

namespace Utils::Time
{
    TEST(GlobalClockTest, DurationIncreasesAfterConstruction)
    {
        GlobalClock clock;

        std::this_thread::sleep_for(15ms);

        EXPECT_GE(clock.GetDurationMillisec(), 5.0);
    }

    TEST(GlobalClockTest, StartResetsDuration)
    {
        GlobalClock clock;

        std::this_thread::sleep_for(25ms);
        clock.Start();

        EXPECT_LT(clock.GetDurationMillisec(), 15.0);
    }

    TEST(GlobalClockTest, UpdateDeltaReportsElapsedTimeSincePreviousUpdate)
    {
        GlobalClock clock;

        std::this_thread::sleep_for(10ms);
        clock.UpdateDelta();

        const double firstDelta = clock.GetDeltaMillisec();
        EXPECT_GE(firstDelta, 5.0);

        std::this_thread::sleep_for(20ms);
        clock.UpdateDelta();

        const double secondDelta = clock.GetDeltaMillisec();
        EXPECT_GE(secondDelta, 10.0);
    }

    TEST(GlobalClockTest, DeltaIsNearZeroImmediatelyAfterUpdate)
    {
        GlobalClock clock;

        clock.UpdateDelta();

        EXPECT_LT(clock.GetDeltaMillisec(), 5.0);
    }
} // namespace Utils::Time

