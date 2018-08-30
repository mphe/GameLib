#ifndef UTILS_LOG_HPP
#define UTILS_LOG_HPP

#include <iostream>

// Older versions of Visual Studio fail compiling this file.
#ifdef DISABLE_LOGGING
#   define LOG(...)
#   define LOG_WARN(...)
#   define LOG_ERROR(...)
#   define LOG_DEBUG(...)
#   define LOG_DEBUG_WARN(...)
#   define LOG_DEBUG_ERROR(...)
#   define LOG_RAW(...)
#   define LOG_DEBUG_RAW(...)
#   define NLOGDEBUG
#else

#ifdef __linux__
#   define LOG_WARNING_PREFIX "\033[1;33mWARNING: \033[0m"
#   define LOG_ERROR_PREFIX "\033[1;31mERROR: \033[0m"
// #   define LOG_DEBUG_PREFIX "\033[;35mDEBUG: \033[0m"
#else
#   define LOG_WARNING_PREFIX "WARNING: "
#   define LOG_ERROR_PREFIX "ERROR: "
// #   define LOG_DEBUG_PREFIX "DEBUG: "
#endif

#define LOG_NORMAL_PREFIX ""
#define LOG_DEBUG_PREFIX "DEBUG: "

// Use like this:
// int x = 5;
// LOG("Some log entry");
// LOG("some variable: ", x);
// LOG_WARN("Something is wrong: ", x, "!");
#define LOG(...)        logutils::logfunc_init(__VA_ARGS__)
#define LOG_WARN(...)   logutils::logfunc_init<logutils::Warning>(__VA_ARGS__)
#define LOG_ERROR(...)  logutils::logfunc_init<logutils::Error, std::cerr>(__VA_ARGS__)

// Same as above but without appending a new line character at the end
#define LOG_RAW(...)        logutils::logfunc_init<logutils::Normal, std::cout, false>(__VA_ARGS__)

// Same as above but will only be displayed when NLOGDEBUG is NOT defined
#ifndef NLOGDEBUG
#   define LOG_DEBUG(...)           logutils::logfunc_init<logutils::Debug>(__VA_ARGS__)
#   define LOG_DEBUG_WARN(...)      logutils::logfunc_init<logutils::DebugWarning>(__VA_ARGS__)
#   define LOG_DEBUG_ERROR(...)     logutils::logfunc_init<logutils::DebugError, std::cerr>(__VA_ARGS__)
#   define LOG_DEBUG_RAW(...)       logutils::logfunc_init<logutils::Debug, std::cout, false>(__VA_ARGS__)
#else // No, no, Mr. Debug no here.
#   define LOG_DEBUG(...)
#   define LOG_DEBUG_WARN(...)
#   define LOG_DEBUG_ERROR(...)
#   define LOG_DEBUG_RAW(...)
#endif

// Use like this:
// int x = 5;
// LOG(LOG_DUMP(x));
#define LOG_DUMP(var) #var, ": ", (var)


namespace logutils
{
    enum LogLevel
    {
        Normal,
        Warning,
        Error,
        Debug,
        DebugWarning,
        DebugError
    };

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

    template<LogLevel level = Normal, std::ostream& stream = std::cout, bool nl = true, class... Args>
    void logfunc_init(const Args&... args)
    {
        switch (level)
        {
            case DebugWarning:
            case Warning:
                stream<<LOG_WARNING_PREFIX;
                break;

            case DebugError:
            case Error:
                stream<<LOG_ERROR_PREFIX;
                break;

            case Normal:
            default:
                stream<<LOG_NORMAL_PREFIX;
                break;
        }
        if (level >= 3)
            stream<<LOG_DEBUG_PREFIX;
        logfunc_join<stream, nl>(args...);
    }
}
#endif

#endif
