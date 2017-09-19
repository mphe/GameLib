#include "gamelib/utils/string.hpp"

namespace gamelib
{
#ifdef _WIN32
    constexpr char oldsep = '/';
#else
    constexpr char oldsep = '\\';
#endif

    std::string& adaptPath(std::string* path)
    {
        for (size_t i = path->find_first_of(oldsep); i != std::string::npos;
                i = path->find_first_of(oldsep, i + 1))
            path[i] = separator;

        return *path;
    }

    std::string& assureDelimiter(std::string* path)
    {
        if (path->back() != separator)
            path->push_back(separator);
        return *path;
    }

    std::string getDirectory(const std::string& path)
    {
        auto pos = path.rfind('/');
        if (pos == std::string::npos)
            return "";
        if (pos == 0)
            return "/";
        return path.substr(0, pos);
    }
}
