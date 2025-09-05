#include "utils/logs/logger.hpp"
#include "utils/time/date.hpp"

#include <cstdarg>
#include <iostream>
#include <string>

namespace Utils::Logs
{
    const std::string Logger::DEBUG   = "\033[0m";
    const std::string Logger::CYAN    = "\033[36m";
    const std::string Logger::YELLOW  = "\033[33m";
    const std::string Logger::RED     = "\033[31m";

    void Logger::Debug(char const* const format, ...)
    {
        va_list args;
        va_start(args, format);

        std::string message = parseArgs(format, args);
        printLog(message, ELogLevel::Debug);
        
        va_end(args);

        LogMessageReceived.Invoke(ELogLevel::Debug, message);
    }

    void Logger::Info(char const* const format, ...) {
        va_list args;
        va_start(args, format);

        std::string message = parseArgs(format, args);
        printLog(message, ELogLevel::Info);

        va_end(args);

        LogMessageReceived.Invoke(ELogLevel::Info, message);
    }

    void Logger::Warning(char const* const format, ...) {
        va_list args;
        va_start(args, format);

        std::string message = parseArgs(format, args);
        printLog(message, ELogLevel::Warning);
        
        va_end(args);

        LogMessageReceived.Invoke(ELogLevel::Warning, message);
    }

    void Logger::Error(char const* const format, ...)
    {
        va_list args;
        va_start(args, format);

        std::string message = parseArgs(format, args);    
        printLog(message, ELogLevel::Error);
    
        va_end(args);
    
        LogMessageReceived.Invoke(ELogLevel::Error, message);
    }

    std::string Logger::parseArgs(char const* const format, va_list& args)
    {
        va_list copy;
        va_copy(copy, args);

        int size = vsnprintf(nullptr, 0, format, copy);
        
        if (size <= 0)
        {
            va_end(copy);
            return "";
        }
        
        std::string buffer(size + 1, '\0');
        vsnprintf(&buffer[0], size + 1, format, args);
        
        va_end(copy);
        return buffer;
    }

    void Logger::printLog(const std::string& message, ELogLevel logLevel)
    {
        std::cout << getLogLevelOutput(logLevel)
                  << "[" << Utils::Time::Date::GetDateAndTime() << "] "
                  << message << DEBUG << std::endl;
    }

    std::string Logger::getLogLevelOutput(ELogLevel logLevel)
    {
        switch (logLevel)
        {
            case ELogLevel::Debug:   return "";
            case ELogLevel::Info:    return CYAN +   "[Info] ";
            case ELogLevel::Warning: return YELLOW + "[Warning] ";
            case ELogLevel::Error:   return RED +    "[Error] ";
            default:                 return "[Uknown] ";
        }

        return "[Unknown] ";
    }
}
