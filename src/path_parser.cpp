#include "utils/path_parser.hpp"

namespace Utils
{
    std::string GetFileName(const std::filesystem::path& path)
    {
        if (!path.has_filename())
            return {};

        auto filename = path.filename();
        return filename.replace_extension().string();
    }
} // namespace Utils

