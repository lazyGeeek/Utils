#include "utils/time/date.hpp"
#include "utils/time/utils.hpp"

namespace Utils::Time
{
    Time GetNow()
    {
        Time time {};
        const auto timeSinceEpoch = SystemClock::now().time_since_epoch();

        // Convert to milliseconds since epoch
        auto msSinceEpoch = DurationCast<Millisecs>(timeSinceEpoch);

        time.Milliseconds = (msSinceEpoch % Millisecs(1000)).count();
        time.Microseconds =
            (DurationCast<Microsecs>(timeSinceEpoch) % Microsecs(1000)).count();
        time.Nanoseconds =
            (DurationCast<Nanosecs>(timeSinceEpoch) % Nanosecs(1000)).count();

        // Truncate to seconds for the main time part
        auto secondsSinceEpoch = DurationCast<Seconds>(msSinceEpoch);

        // Format the date and time as UTC
        std::time_t timestamp = secondsSinceEpoch.count();
        std::tm buf {};
#ifdef _WIN32
        gmtime_s(&buf, &timestamp);
#else
        gmtime_r(&timestamp, &buf);
#endif

        time.Time = buf;
        return time;
    }

    std::string GetDate()
    {
        const auto time = GetNow();

        std::ostringstream ss;
        ss << std::put_time(&time.Time, "%Y-%m-%d");
        return ss.str();
    }

    std::string GetTime()
    {
        const auto time = GetNow();

        std::ostringstream ss;
        ss << std::put_time(&time.Time, "%H:%M:%S");

        return ss.str();
    }

    std::string GetDateAndTime()
    {
        const auto time = GetNow();

        std::ostringstream ss;
        ss << std::put_time(&time.Time, "%Y-%m-%d %H:%M:%S") << '.'
           << std::setw(3) << std::setfill('0') << time.Milliseconds;

        return ss.str();
    }
} // namespace Utils::Time

