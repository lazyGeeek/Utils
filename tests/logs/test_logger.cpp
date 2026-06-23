#include "utils/logs/log_level.hpp"
#include "utils/logs/logger.hpp"
#include "gtest/gtest.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>

namespace Utils::Logs
{
    static std::string ReadFileAll(const std::filesystem::path& file)
    {
        std::ifstream ifs(file);
        std::ostringstream ss;
        ss << ifs.rdbuf();
        return ss.str();
    }

    // Build a regex that matches the Logger's line format with strict ANSI
    // color prefix. Pattern: ^[timestamp] [LogLevel] message\n$
    static std::string BuildStrictLinePattern(ELogLevel lvl,
                                              std::string_view message)
    {
        const std::string lvlName = LogName(lvl);

        // Timestamp produced by nowIso8601(): YYYY-MM-DD HH:MM:SS.uuu
        const std::string timestampPattern =
            R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{3})";

        return R"(\[)" + timestampPattern + R"(\] \[)" + lvlName + R"(\] )" +
               std::string(message);
    }

    static void CheckRegex(std::string input, std::string pattern)
    {
        EXPECT_TRUE(std::regex_match(
            input, std::regex(pattern, std::regex::ECMAScript)))
            << "Line did not match expected strict format\n"
            << "Pattern: " << pattern << "\nLine: " << input;
    }

    TEST(LoggerGeneratTest, MinLogLevel)
    {
        Logger logger;
        logger.EnableConsole(false);
        std::string captured = "";

        logger.SetCallback(
            [&captured](std::string_view s) { captured = std::string(s); });

        logger.SetMinLogLevel(ELogLevel::Warning);

        logger.Debug("Debug Message");
        ASSERT_TRUE(captured.empty()) << "Logger didn't count min log level";

        logger.Warning("Warning Message");
        ASSERT_FALSE(captured.empty())
            << "Logger incorrectly interpret minimal log level";

        captured = "";
        logger.SetMinLogLevel(ELogLevel::Info);
        logger.Debug("Debug Message {}", 2);
        ASSERT_FALSE(captured.empty());
    }

    TEST(LoggerExactFormatTest, CallbackLineMatchesStrictFormat)
    {
        Logger logger;
        std::string captured = "";

        logger.SetCallback(
            [&captured](std::string_view s) { captured = std::string(s); });

        logger.Info("ExactFormatStrict {}", 1);

        ASSERT_FALSE(captured.empty());

        CheckRegex(captured, BuildStrictLinePattern(ELogLevel::Info,
                                                    "ExactFormatStrict 1"));
    }

    TEST(LoggerExactFormatTest, FileLineMatchesStrictFormat)
    {
        const auto tmpDir = std::filesystem::temp_directory_path();
        auto p = tmpDir / "logger_exact_format_strict_test.txt";
        std::filesystem::remove(p);

        {
            Logger logger;

            ASSERT_TRUE(logger.SetFile(p, true));
            logger.EnableConsole(false);
            logger.SetCallback(nullptr);

            logger.Warning("WarnMessageStrict {}", 42);

            std::string content = ReadFileAll(p);
            ASSERT_FALSE(content.empty());

            auto pattern = BuildStrictLinePattern(ELogLevel::Warning,
                                                  "WarnMessageStrict 42");

            std::string filePattern = pattern + R"(\n)";

            CheckRegex(content, filePattern);
        }

        std::filesystem::remove(p);
    }

    TEST(LoggerExactFormatTest, ConsoleLineMatchesStrictFormat)
    {
        const auto tmp = std::filesystem::temp_directory_path() /
                         "logger_exact_stdout_strict.txt";
        std::filesystem::remove(tmp);

        {
            Logger logger;
            FILE* f = nullptr;

#ifdef _WIN32
            auto error = fopen_s(&f, tmp.string().c_str(), "wb");
            ASSERT_NE(f, nullptr);
            ASSERT_EQ(error, 0);

            auto old = _dup(_fileno(stdout));
            _dup2(_fileno(f), _fileno(stdout));
#else
            f = fopen(tmp.string().c_str(), "wb");
            ASSERT_NE(f, nullptr);

            auto old = dup(fileno(stdout));
            dup2(fileno(f), fileno(stdout));
#endif

            logger.SetCallback(nullptr);
            logger.EnableConsole(true);

            logger.Error("ErrExactStrict {}", 7);

            fflush(stdout);

#ifdef _WIN32
            _dup2(old, _fileno(stdout));
            _close(old);
#else
            dup2(old, fileno(stdout));
            close(old);
#endif

            fclose(f);

            std::string captured = ReadFileAll(tmp);

            ASSERT_FALSE(captured.empty());

            auto pattern =
                BuildStrictLinePattern(ELogLevel::Error, "ErrExactStrict 7");

            std::string filePattern =
                R"(\x1b\[31m)" + pattern + R"(\x1b\[0m\n)";

            CheckRegex(captured, filePattern);
        }

        std::filesystem::remove(tmp);
    }

    // ------------------ Concurrency test ------------------

    // Helper: splits content into vector of lines including trailing
    // newline.static
    std::vector<std::string> SplitLinesWithNewline(const std::string& content)
    {
        std::vector<std::string> lines;
        std::istringstream iss(content);
        std::string ln;

        while (std::getline(iss, ln)) {
            // getline removes newline; re-add it to match logger format
            if (!ln.empty())
                lines.push_back(ln + "\n");
        }

        return lines;
    }

    // Concurrent logging: spawn many threads, each logs many messages.
    // Use callback to collect lines and also write to a file. After
    // join, compare file contents vs callback.
    TEST(LoggerConcurrencyTest, FileMatchesCallbackAndNoInterleaving)
    {
        const auto tmpDir = std::filesystem::temp_directory_path();
        auto filePath = tmpDir / "logger_concurrency_match_test.txt";
        std::filesystem::remove(filePath);

        {
            Logger logger;
            std::vector<std::string> received;
            std::mutex vecMutex;

            logger.EnableConsole(false); // avoid stdout noise

            ASSERT_TRUE(logger.SetFile(filePath));

            auto callback = [&received, &vecMutex](std::string_view s) {
                std::lock_guard<std::mutex> lock(vecMutex);
                received.emplace_back(std::string(s) + "\n");
            };

            logger.SetCallback(callback);

            const int numThreads = 12;
            const int msgsPerThread = 300;
            std::vector<std::thread> threads;
            threads.reserve(numThreads);

            // Each thread will produce messages: "T{id}-M{j}"
            for (int t = 0; t < numThreads; ++t) {
                threads.emplace_back(std::thread([&logger, t, msgsPerThread] {
                    for (int m = 0; m < msgsPerThread; ++m) {
                        logger.Info("T{}-M{}", t, m);
                    }
                }));
            }

            for (auto& th : threads)
                th.join();

            // Wait briefly to ensure file flushes (WriteRecord flushes
            // fileStream, but be conservative)
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            // Read file contents
            std::string fileContent = ReadFileAll(filePath);
            auto fileLines = SplitLinesWithNewline(fileContent);

            // Sanity: total counts should match
            const size_t expectedTotal = static_cast<size_t>(numThreads) *
                                         static_cast<size_t>(msgsPerThread);
            ASSERT_EQ(received.size(), expectedTotal);
            ASSERT_EQ(fileLines.size(), expectedTotal);

            // Build multisets (maps count->occurrences) for strict comparison
            std::map<std::string, int> callbackCounts;
            {
                std::lock_guard<std::mutex> lock(vecMutex);
                for (const auto& line : received) {
                    callbackCounts[line]++;
                }
            }

            std::map<std::string, int> fileCounts;
            {
                std::lock_guard<std::mutex> lock(vecMutex);
                for (const auto& line : fileLines) {
                    fileCounts[line]++;
                }
            }

            // The two multisets should be identical.
            EXPECT_EQ(callbackCounts.size(), fileCounts.size());
            EXPECT_EQ(callbackCounts, fileCounts);

            // Additionally, verify each received line matches strict format
            // and extract message token integrity
            std::set<std::string> tokens;
            for (const auto& kv : callbackCounts) {
                const std::string& line = kv.first;
                // Extract message part by finding "] [Info] "
                const std::string marker = "] [INFO] ";
                auto pos = line.find(marker);
                ASSERT_NE(pos, std::string::npos);
                std::string messagePart = line.substr(pos + marker.size());
                // messagePart includes trailing newline; strip it
                if (!messagePart.empty() && messagePart.back() == '\n')
                    messagePart.pop_back();

                auto pattern =
                    BuildStrictLinePattern(ELogLevel::Info, messagePart) +
                    R"(\n)";

                CheckRegex(line, pattern);

                // Collect tokens for further sample checks
                tokens.insert(messagePart);
            }

            // Check that some expected tokens exist
            EXPECT_NE(tokens.find("T0-M0"), tokens.end());
            EXPECT_NE(tokens.find("T0-M1"), tokens.end());
            EXPECT_NE(tokens.find("T1-M0"), tokens.end());
            EXPECT_NE(tokens.find(("T" + std::to_string(numThreads - 1) + "-M" +
                                   std::to_string(msgsPerThread - 1))),
                      tokens.end());
        }

        // cleanup
        std::filesystem::remove(filePath);
    }
} // namespace Utils::Logs

