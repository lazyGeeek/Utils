#pragma once
#ifndef UTILS_LOGS_LOGGER_HPP_
#define UTILS_LOGS_LOGGER_HPP_

#include <string>

#include "log_level.hpp"
#include "utils/eventing/event.hpp"

#define LOG_DEBUG(format, ...)   Utils::Logs::Logger::Debug(format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)    Utils::Logs::Logger::Info(format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) Utils::Logs::Logger::Warning(format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...)   Utils::Logs::Logger::Error(format, ##__VA_ARGS__)

namespace Utils::Logs
{
    class Logger
    {
    public:
        Logger() = delete;

        Logger(const Logger& other)             = delete;
        Logger(Logger&& other)                  = delete;
        Logger& operator=(const Logger& other)  = delete;
        Logger& operator=(const Logger&& other) = delete;

        static void Debug(char const* const format, ...);
        static void Info(char const* const format, ...);
        static void Warning(char const* const format, ...);
        static void Error(char const* const format, ...);

        static inline Utils::Eventing::Event<ELogLevel, const std::string&> LogMessageReceived;

    private:
        static std::string parseArgs(char const* const format, va_list& args);

        static std::string getLogLevelOutput(ELogLevel logLevel = ELogLevel::Debug);
        static void printLog(const std::string& message, ELogLevel logLevel = ELogLevel::Info);

        static const std::string DEBUG;
        static const std::string RED;
        static const std::string YELLOW;
        static const std::string CYAN;
    };
}

#endif // UTILS_LOGS_LOGGER_HPP_
