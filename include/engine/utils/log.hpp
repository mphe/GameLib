#ifndef UTILS_LOG_HPP
#define UTILS_LOG_HPP

#include <iostream>

#define LOG_NORMAL_PREFIX ""
#define LOG_WARNING_PREFIX "WARNING: "
#define LOG_ERROR_PREFIX "ERROR: "
#define LOG_DEBUG_PREFIX "DEBUG: "

// Use like this:
// int x = 5;
// LOG("Some log entry");
// LOG("some variable: ", x);
// LOG_WARN("Something is wrong: ", x, "!");
#define LOG(...)        logutils::logfunc_join<std::cout, true>(LOG_NORMAL_PREFIX, ##__VA_ARGS__)
#define LOG_WARN(...)   logutils::logfunc_join<std::cout, true>(LOG_WARNING_PREFIX, ##__VA_ARGS__)
#define LOG_ERROR(...)  logutils::logfunc_join<std::cerr, true>(LOG_ERROR_PREFIX, ##__VA_ARGS__)

// Same as above but without appending a new line character at the end
#define LOG_RAW(...)        logutils::logfunc_join<std::cout, false>(LOG_NORMAL_PREFIX, ##__VA_ARGS__)
#define LOG_WARN_RAW(...)   logutils::logfunc_join<std::cout, false>(LOG_WARNING_PREFIX, ##__VA_ARGS__)
#define LOG_ERROR_RAW(...)  logutils::logfunc_join<std::cerr, false>(LOG_ERROR_PREFIX, ##__VA_ARGS__)

// Same as above but will only be displayed when NLOGDEBUG is NOT defined
#ifndef NLOGDEBUG
#   define LOG_DEBUG(...)       LOG(LOG_DEBUG_PREFIX, ##__VA_ARGS__)
#   define LOG_DEBUG_WARN(...)  LOG_WARN(LOG_DEBUG_PREFIX, ##__VA_ARGS__)
#   define LOG_DEBUG_ERROR(...) LOG_ERROR(LOG_DEBUG_PREFIX, ##__VA_ARGS__)

#   define LOG_DEBUG_RAW(...)       LOG_RAW(LOG_DEBUG_PREFIX, ##__VA_ARGS__)
#   define LOG_DEBUG_WARN_RAW(...)  LOG_WARN_RAW(LOG_DEBUG_PREFIX, ##__VA_ARGS__)
#   define LOG_DEBUG_ERROR_RAW(...) LOG_ERROR_RAW(LOG_DEBUG_PREFIX, ##__VA_ARGS__)
#else // No, no, Mr. Debug no here.
#   define LOG_DEBUG(...)
#   define LOG_DEBUG_WARN(...)
#   define LOG_DEBUG_ERROR(...)

#   define LOG_DEBUG_RAW(...)
#   define LOG_DEBUG_WARN_RAW(...)
#   define LOG_DEBUG_ERROR_RAW(...)
#endif

// Use like this:
// int x = 5;
// LOG(LOG_DUMP(x));
#define LOG_DUMP(var) #var, ": ", (var)


namespace logutils
{
    template<std::ostream& stream, bool nl>
    void logfunc_join()
    {
        if (nl)
            stream<<"\n";
        else
            stream.flush();
    }

    template<std::ostream& stream, bool nl, class T, class... Args>
    void logfunc_join(const T& arg, const Args&... rest)
    {
       stream<<arg;
       logfunc_join<stream, nl>(rest...);
    }
}

#endif
