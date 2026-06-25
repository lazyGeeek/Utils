#include "utils/logs/logger.hpp"
#include "utils/time/date.hpp"

#include <iostream>

namespace Utils::Logs
{
    Logger::~Logger()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_fileStream.is_open())
            m_fileStream.close();
    }

    void Logger::EnableConsole(bool console) noexcept
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_console = console;
    }

    bool Logger::SetFile(std::filesystem::path path, bool append)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::ofstream tmp;
        tmp.open(path,
                 std::ios::out | (append ? std::ios::app : std::ios::trunc));
        if (!tmp)
            return false;

        m_fileStream.swap(tmp);
        m_filePath = std::move(path);

        return true;
    }

    void Logger::SetCallback(std::function<void(std::string_view)> callback)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_callback = std::move(callback);
    }

    void Logger::SetMinLogLevel(ELogLevel minLevel) noexcept
    {
        m_minLevel.store(minLevel, std::memory_order_relaxed);
    }

    void Logger::writeRecord(ELogLevel level, std::string&& message)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        const auto timestamp = Time::GetDateAndTime();
        const auto lvl = LogName(level);
        const auto color = LogColor(level);

        auto line = std::format("[{}] [{}] {}", timestamp, lvl, message);

        // Acquire sink mutex and then write to sinks.
        if (m_callback)
            m_callback(line);

        if (m_console) {
            auto consoleLine =
                std::format("{}{}{}\n", color, line, LogColor(ELogLevel::Info));
            std::cout << consoleLine;
        }

        if (m_fileStream.is_open()) {
            m_fileStream << line << '\n';
            m_fileStream.flush();
        }
    }

} // namespace Utils::Logs
