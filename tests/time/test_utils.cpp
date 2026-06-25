#include <gtest/gtest.h>

#include "utils/time/utils.hpp"

namespace Utils::Time
{
    TEST(DurationCast, Seconds_Zero)
    {
        Duration d { 0.0 };
        auto out = Utils::Time::DurationCast<std::chrono::seconds>(d);
        EXPECT_EQ(out.count(), 0);
    }

    TEST(DurationCast, Millisecs_Zero)
    {
        Duration d { 0.0 };
        auto out = Utils::Time::DurationCast<std::chrono::milliseconds>(d);
        EXPECT_EQ(out.count(), 0);
    }

    TEST(DurationCast, Microsecs_Zero)
    {
        Duration d { 0.0 };
        auto out = Utils::Time::DurationCast<std::chrono::microseconds>(d);
        EXPECT_EQ(out.count(), 0);
    }

    TEST(DurationCast, Nanosecs_Zero)
    {
        Duration d { 0.0 };
        auto out = Utils::Time::DurationCast<std::chrono::nanoseconds>(d);
        EXPECT_EQ(out.count(), 0);
    }

    TEST(DurationCast, Seconds_PositiveExactFractional)
    {
        Duration d { 1.25 }; // seconds
        auto out = Utils::Time::DurationCast<std::chrono::seconds>(d);
        EXPECT_EQ(out.count(), 1); // duration_cast truncates toward zero
    }

    TEST(DurationCast, Millisecs_PositiveExactFractional)
    {
        Duration d { 1.25 }; // seconds -> 1250 ms
        auto out = Utils::Time::DurationCast<std::chrono::milliseconds>(d);
        EXPECT_EQ(out.count(), 1250);
    }

    TEST(DurationCast, Microsecs_PositiveExactFractional)
    {
        Duration d { 1.25 }; // seconds -> 1.25e6 us
        auto out = Utils::Time::DurationCast<std::chrono::microseconds>(d);
        EXPECT_EQ(out.count(), 1250000);
    }

    TEST(DurationCast, Nanosecs_PositiveExactFractional)
    {
        Duration d { 1.25 }; // seconds -> 1.25e9 ns
        auto out = Utils::Time::DurationCast<std::chrono::nanoseconds>(d);
        EXPECT_EQ(out.count(), 1250000000LL);
    }

    TEST(DurationCast, Negative_TruncatesTowardZero)
    {
        Duration d { -1.25 }; // seconds
        auto outS = Utils::Time::DurationCast<std::chrono::seconds>(d);
        auto outMS = Utils::Time::DurationCast<std::chrono::milliseconds>(d);

        EXPECT_EQ(outS.count(), -1);     // trunc toward zero
        EXPECT_EQ(outMS.count(), -1250); // exact conversion at this value
    }

    TEST(DurationCast, LargeValue_DoesNotOverflowForExpectedRange)
    {
        // Pick a value that should fit comfortably in typical int64 counts for
        // all three.
        Duration d { 12345.678 }; // seconds
        auto s = Utils::Time::DurationCast<std::chrono::seconds>(d);
        auto ms = Utils::Time::DurationCast<std::chrono::milliseconds>(d);
        auto us = Utils::Time::DurationCast<std::chrono::microseconds>(d);

        EXPECT_EQ(s.count(), static_cast<long long>(12345)); // trunc
        EXPECT_EQ(ms.count(), static_cast<long long>(
                                  12345.678 * 1e3)); // ms exact after cast
        EXPECT_EQ(us.count(), static_cast<long long>(
                                  12345.678 * 1e6)); // us exact after cast
    }

    TEST(DurationCast, ReturnType_IsCorrect)
    {
        Duration d { 1.0 };
        static_assert(
            std::is_same_v<
                decltype(Utils::Time::DurationCast<std::chrono::seconds>(d)),
                std::chrono::seconds>);
        static_assert(std::is_same_v<decltype(Utils::Time::DurationCast<
                                              std::chrono::milliseconds>(d)),
                                     std::chrono::milliseconds>);
        static_assert(std::is_same_v<decltype(Utils::Time::DurationCast<
                                              std::chrono::microseconds>(d)),
                                     std::chrono::microseconds>);
        static_assert(std::is_same_v<decltype(Utils::Time::DurationCast<
                                              std::chrono::nanoseconds>(d)),
                                     std::chrono::nanoseconds>);
    }
} // namespace Utils::Time

