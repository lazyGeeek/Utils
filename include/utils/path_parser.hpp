#pragma once
#ifndef UTILS_PATH_PARSER_HPP_
#define UTILS_PATH_PARSER_HPP_

#include <filesystem>
#include <string>

namespace Utils
{
    class PathParser
    {
    public:
        PathParser() = delete;

        static std::string GetFileName(const std::filesystem::path& path);
    };
}

#endif // UTILS_PATH_PARSER_HPP_
