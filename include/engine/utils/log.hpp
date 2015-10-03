#ifndef UTILS_LOG_HPP
#define UTILS_LOG_HPP

#include <iostream>

namespace logutils
{
// Use like this:
// int x = 5;
// LOG("Some log entry");
// LOG("some variable: ", x);
// LOG_WARN("Something is wrong: ", x, "!");
#define LOG logutils::logfunc<logutils::normal, std::cout, true>
#define LOG_WARN logutils::logfunc<logutils::warning, std::cout, true>
#define LOG_ERROR logutils::logfunc<logutils::error, std::cerr, true>

// Same as above but without appending a new line character at the end
#define LOG_RAW logutils::logfunc<logutils::normal, std::cout, false>
#define LOG_WARN_RAW  logutils::logfunc<logutils::warning, std::cout, false>
#define LOG_ERROR_RAW  logutils::logfunc<logutils::error, std::cerr, false>

// Same as above but will only be displayed when NLOGDEBUG is NOT defined
#ifndef NLOGDEBUG
#   define LOG_DEBUG logutils::logfunc<logutils::debug, std::cout, true>
#   define LOG_DEBUG_RAW logutils::logfunc<logutils::debug, std::cout, false>
#else // No, no, Mr. Debug no here.
#   define LOG_DEBUG(...)
#   define LOG_DEBUG_RAW(...)
#endif

// Use like this:
// int x = 5;
// LOG(LOG_DUMP(x));
#define LOG_DUMP(var) #var, ": ", (var)


    enum Severity
    {
        normal,
        debug,
        warning,
        error
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

    template<Severity type, std::ostream& stream, bool nl, class... Args>
    void logfunc(const Args&... args)
    {
        switch (type)
        {
            case debug:
                stream<<"DEBUG: ";
                break;
            case warning:
                stream<<"WARNING: ";
                break;
            case error:
                stream<<"ERROR: ";
                break;
        }
        logfunc_join<stream, nl>(args...);
    }
}

#endif
