#pragma once
#ifndef UTILS_TIME_DATE_HPP_
#define UTILS_TIME_DATE_HPP_

#include <chrono>
#include <ctime>
#include <string>

namespace Utils::Time
{
    class Date
    {
    public:
        Date() = delete;

        static inline std::string GetDate()
        {
            tm timeInfo = getTimeInfo();
            char result[20];
            size_t size = std::strftime(result, sizeof(result), "%Y-%m-%d", &timeInfo);
            std::string timestamp(result, size);

            return timestamp;
        }

        static inline std::string GetTime()
        {
            tm timeInfo = getTimeInfo();
            char result[20];
            size_t size = std::strftime(result, sizeof(result), "%H:%M:%S", &timeInfo);
            std::string timestamp(result, size);

            return timestamp;
        }

        static inline std::string GetDateAndTime()
        {
            return GetDate() + " " + GetTime();
        }

    private:
        static inline tm getTimeInfo()
        {
            std::time_t currentTime = std::time(nullptr);
            tm* timeInfo = localtime(&currentTime);
            return *timeInfo;
        }
    };
}

#endif // UTILS_TIME_DATE_HPP_
