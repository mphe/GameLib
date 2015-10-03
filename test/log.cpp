#include "utils/log.hpp"

int main()
{
    LOG("Normal log entry.");
    LOG_WARN("Warning log entry");
    LOG_ERROR("Error log entry");
    LOG_DEBUG("Debug log entry");

    LOG_RAW("Normal log entry without newline");
    LOG_WARN_RAW("Warning log entry without newline");
    LOG_ERROR_RAW("Error log entry without newline");
    LOG_DEBUG_RAW("Debug log entry without newline");

    int x = 5;
    LOG("\nA variable: ", LOG_DUMP(x));

    return 0;
}
