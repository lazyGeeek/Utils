#pragma once

#include <cstdint>
#include <string>

namespace Utils::Logs
{
    enum class ELogLevel : std::uint8_t
    {
        Info,
        Debug,
        Warning,
        Error
    };

    inline std::string LogName(ELogLevel level) noexcept
    {
        switch (level) {
        case ELogLevel::Debug:
            return "DEBUG";
        case ELogLevel::Warning:
            return "WARNING";
        case ELogLevel::Error:
            return "ERROR";
        case ELogLevel::Info:
        default:
            return "INFO";
        }

        return "UNKNOWN";
    }

    // NONE    "\x1b[0m";
    // RED     "\x1b[31m";
    // YELLOW  "\x1b[33m";
    // CYAN    "\x1b[36m";
    inline std::string LogColor(ELogLevel level) noexcept
    {
        switch (level) {
        case ELogLevel::Debug:
            return "\x1b[36m";
        case ELogLevel::Warning:
            return "\x1b[33m";
        case ELogLevel::Error:
            return "\x1b[31m";
        case ELogLevel::Info:
        default:
            return "\x1b[0m";
        }

        return "\x1b[0m";
    }

} // namespace Utils::Logs
