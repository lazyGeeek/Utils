#pragma once

#include "utils/logs/log_level.hpp"
#include "utils/non_copyable.hpp"

#include <atomic>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <mutex>
#include <string>
#include <string_view>
#include <utility>

namespace Utils::Logs
{
    class Logger : public NonCopyable
    {
    public:
        Logger() = default;
        ~Logger() override;

        // Configure sinks
        void EnableConsole(bool console = true) noexcept;

        // Configure file sink (opens in append mode). Returns false on
        // failure.
        bool SetFile(std::filesystem::path path, bool append = true);
        void SetCallback(std::function<void(std::string_view)> callback);
        void SetMinLogLevel(ELogLevel minLevel) noexcept;

        template <class... Args>
        void Log(ELogLevel level, std::string_view fmt, Args&&... args)
        {
            if (level < m_minLevel.load(std::memory_order_relaxed))
                return;

            auto msg = std::vformat(fmt, std::make_format_args(args...));

            writeRecord(level, std::move(msg));
        }

        template <class... Args>
        void Info(std::string_view fmt, Args&&... args)
        {
            Log(ELogLevel::Info, fmt, std::forward<Args>(args)...);
        }

        template <class... Args>
        void Debug(std::string_view fmt, Args&&... args)
        {
            Log(ELogLevel::Debug, fmt, std::forward<Args>(args)...);
        }

        template <class... Args>
        void Warning(std::string_view fmt, Args&&... args)
        {
            Log(ELogLevel::Warning, fmt, std::forward<Args>(args)...);
        }

        template <class... Args>
        void Error(std::string_view fmt, Args&&... args)
        {
            Log(ELogLevel::Error, fmt, std::forward<Args>(args)...);
        }

    private:
        static std::string nowIso8601();
        void writeRecord(ELogLevel level, std::string&& message);

        std::mutex m_mutex;
        std::ofstream m_fileStream;
        std::filesystem::path m_filePath;
        bool m_console = true;

        std::function<void(std::string_view)> m_callback;
        std::atomic<ELogLevel> m_minLevel { ELogLevel::Info };
    };
} // namespace Utils::Logs
