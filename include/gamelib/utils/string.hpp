#ifndef GAMELIB_STRING_UTILS
#define GAMELIB_STRING_UTILS

#include <sstream>
#include <string>

namespace gamelib
{
    namespace internal
    {
        template <class T>
        void joinString(std::stringstream& ss, const T& arg)
        {
            ss << arg;
        }

        template <class T, class... Args>
        void joinString(std::stringstream& ss, const T& arg, Args&&... rest)
        {
            ss << arg;
            joinString(ss, std::forward<Args>(rest)...);
        }
    }

    template <class... Args>
    std::string join(Args&&... args)
    {
        std::stringstream ss;
        internal::joinString(ss, std::forward<Args>(args)...);
        return ss.str();
    }

    // Replace '/' or '\' with each other, depending on which platform the
    // code is compiled. The algorithm works in-place and returns a reference
    // to the string.
    std::string& adaptPath(std::string& path);
}

#endif
