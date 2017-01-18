#include "gamelib/utils/string.hpp"

namespace gamelib
{
    std::string& adaptPath(std::string& path)
    {
#ifdef _WIN32
        constexpr char oldsep = '/';
        constexpr char newsep = '\\';
#else
        constexpr char newsep = '/';
        constexpr char oldsep = '\\';
#endif

        for (size_t i = path.find_first_of(oldsep); i != std::string::npos;
                i = path.find_first_of(oldsep, i + 1))
            path[i] = newsep;

        return path;
    }
}
