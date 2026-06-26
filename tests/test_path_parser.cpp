#include <gtest/gtest.h>

#include "utils/path_parser.hpp"

namespace Utils
{
    TEST(PathParserTest, ReturnsEmptyStringWhenPathHasNoFilename)
    {
        EXPECT_EQ(GetFileName(std::filesystem::path {}), "");
        EXPECT_EQ(GetFileName(std::filesystem::path { "/" }), "");
        EXPECT_EQ(GetFileName(std::filesystem::path { "C:/" }), "");
        EXPECT_EQ(GetFileName(std::filesystem::path { "folder/" }), "");
    }

    TEST(PathParserTest, StripsExtensionFromFilename)
    {
        EXPECT_EQ(GetFileName(std::filesystem::path { "file.txt" }), "file");
        EXPECT_EQ(GetFileName(std::filesystem::path { "/path/to/file.txt" }),
                  "file");
        EXPECT_EQ(GetFileName(std::filesystem::path { "C:/path/to/file.txt" }),
                  "file");
    }

    TEST(PathParserTest, ReturnsFilenameWhenNoExtensionPresent)
    {
        EXPECT_EQ(GetFileName(std::filesystem::path { "README" }), "README");
        EXPECT_EQ(GetFileName(std::filesystem::path { "/var/log/README" }),
                  "README");
    }

    TEST(PathParserTest, RemovesOnlyTheFinalExtension)
    {
        EXPECT_EQ(GetFileName(std::filesystem::path { "archive.tar.gz" }),
                  "archive.tar");
        EXPECT_EQ(GetFileName(std::filesystem::path { "name.with.dots.txt" }),
                  "name.with.dots");
    }

    TEST(PathParserTest, HandlesHiddenFiles)
    {
        EXPECT_EQ(GetFileName(std::filesystem::path { ".gitignore" }),
                  ".gitignore");
        EXPECT_EQ(GetFileName(std::filesystem::path { ".config.json" }),
                  ".config");
    }
} // namespace Utils

