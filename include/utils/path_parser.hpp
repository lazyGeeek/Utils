#pragma once
#include <filesystem>
#include <string>

namespace Utils
{
    std::string GetFileName(const std::filesystem::path& path);
}

