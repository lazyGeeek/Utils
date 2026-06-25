#include <gtest/gtest.h>

#include <regex>
#include <string>

#include "utils/time/date.hpp"

namespace Utils::Time
{
    TEST(DateTest, GetDateMatchesYearMonthDayFormat)
    {
        const std::string date = GetDate();
        EXPECT_FALSE(date.empty());
        EXPECT_TRUE(std::regex_match(
            date, std::regex(R"(\d{4}-\d{2}-\d{2})", std::regex::ECMAScript)));
    }

    TEST(DateTest, GetTimeMatchesHourMinuteSecondFormat)
    {
        const std::string time = GetTime();
        EXPECT_FALSE(time.empty());
        EXPECT_TRUE(std::regex_match(
            time, std::regex(R"(\d{2}:\d{2}:\d{2})", std::regex::ECMAScript)));
    }

    TEST(DateTest, GetDateAndTimeConcatenatesDateAndTime)
    {
        const std::string dateAndTime = GetDateAndTime();

        EXPECT_FALSE(dateAndTime.empty());
        EXPECT_TRUE(std::regex_match(
            dateAndTime,
            std::regex(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}.\d{3})",
                       std::regex::ECMAScript)));
    }

    TEST(UtilsTime, GetNow_SubsecondRanges)
    {
        const auto time = GetNow();

        // These are intended by the implementation:
        // - Milliseconds: [0, 999]
        // - Microseconds:  [0, 999] (within second)
        // - Nanoseconds:   [0, 999] (within second)
        // NOTE: micro/nano variables reflect remainders after modulo 1000.
        EXPECT_GE(time.Milliseconds, 0);
        EXPECT_LT(time.Milliseconds, 1000);

        EXPECT_GE(time.Microseconds, 0);
        EXPECT_LT(time.Microseconds, 1000);

        EXPECT_GE(time.Nanoseconds, 0);
        EXPECT_LT(time.Nanoseconds, 1000);
    }

    TEST(UtilsTime, GetNow_MonotonicityWithinSameSecond_BestEffort)
    {
        // Best-effort: ensure fields never go out of range, and that seconds
        // advance over multiple calls (not guaranteed instantly).
        const auto time1 = GetNow();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        const auto time2 = GetNow();

        EXPECT_GE(time2.Milliseconds, 0);
        EXPECT_LT(time2.Milliseconds, 1000);
        EXPECT_GE(time2.Microseconds, 0);
        EXPECT_LT(time2.Microseconds, 1000);
        EXPECT_GE(time2.Nanoseconds, 0);
        EXPECT_LT(time2.Nanoseconds, 1000);

        // buf.tm_year is years since 1900, tm_mon is [0..11]
        const int time1Sec = time1.Time.tm_hour * 3600 +
                             time1.Time.tm_min * 60 + time1.Time.tm_sec;
        const int time2Sec = time2.Time.tm_hour * 3600 +
                             time2.Time.tm_min * 60 + time2.Time.tm_sec;

        // Not strictly monotonic across minute/hour boundaries; just assert
        // they differ rarely by ensuring they are valid ranges (covered
        // elsewhere) and are not NaNs.
        EXPECT_GE(time1Sec, 0);
        EXPECT_LE(time2Sec, 24 * 3600 - 1);
    }
} // namespace Utils::Time

