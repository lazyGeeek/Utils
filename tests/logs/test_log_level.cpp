#include <gtest/gtest.h>

#include "utils/logs/log_level.hpp"

TEST(LogLevelTest, LogNameReturnsExpectedLabels)
{
    EXPECT_EQ(Utils::Logs::LogName(Utils::Logs::ELogLevel::Info), "INFO");
    EXPECT_EQ(Utils::Logs::LogName(Utils::Logs::ELogLevel::Debug), "DEBUG");
    EXPECT_EQ(Utils::Logs::LogName(Utils::Logs::ELogLevel::Warning), "WARNING");
    EXPECT_EQ(Utils::Logs::LogName(Utils::Logs::ELogLevel::Error), "ERROR");
}

TEST(LogLevelTest, LogColorReturnsAnsiEscapeCodes)
{
    EXPECT_EQ(Utils::Logs::LogColor(Utils::Logs::ELogLevel::Info), "\x1b[0m");
    EXPECT_EQ(Utils::Logs::LogColor(Utils::Logs::ELogLevel::Debug), "\x1b[36m");
    EXPECT_EQ(Utils::Logs::LogColor(Utils::Logs::ELogLevel::Warning),
              "\x1b[33m");
    EXPECT_EQ(Utils::Logs::LogColor(Utils::Logs::ELogLevel::Error), "\x1b[31m");
}

TEST(LogLevelTest, EnumValuesAreDistinct)
{
    EXPECT_NE(static_cast<std::uint8_t>(Utils::Logs::ELogLevel::Info),
              static_cast<std::uint8_t>(Utils::Logs::ELogLevel::Debug));
    EXPECT_NE(static_cast<std::uint8_t>(Utils::Logs::ELogLevel::Warning),
              static_cast<std::uint8_t>(Utils::Logs::ELogLevel::Error));
}
