#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "utils/time/timer.hpp"

using namespace std::chrono_literals;

namespace Utils::Time
{
    TEST(ClockTest, ElapsedTimeIncreasesAfterConstruction)
    {
        Timer timer;

        std::this_thread::sleep_for(10ms);

        EXPECT_GE(timer.GetMilliseconds(), 5);
        EXPECT_GE(timer.GetMicroseconds(), timer.GetMilliseconds() * 1000);
        EXPECT_GE(timer.GetSeconds(), 0);
    }

    TEST(ClockTest, StartResetsElapsedTime)
    {
        Timer timer;

        std::this_thread::sleep_for(20ms);
        timer.Start();

        EXPECT_LT(timer.GetMilliseconds(), 15);
    }

    TEST(ClockTest, TimeUnitsAreConsistent)
    {
        Timer timer;

        std::this_thread::sleep_for(50ms);

        const int64_t milliseconds = timer.GetMilliseconds();
        const int64_t microseconds = timer.GetMicroseconds();

        EXPECT_NEAR(static_cast<double>(microseconds),
                    static_cast<double>(milliseconds) * 1000.0, 2000.0);
    }
} // namespace Utils::Time

