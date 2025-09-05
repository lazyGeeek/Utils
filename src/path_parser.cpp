#include "utils/path_parser.hpp"

#include <algorithm>

namespace Utils
{
    std::string PathParser::GetFileName(const std::filesystem::path& path)
    {
        if (!path.has_filename())
            return "";

        std::filesystem::path filename = path.filename();
        return filename.replace_extension("").string();
    }
}
