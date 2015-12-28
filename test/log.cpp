#include "gamelib/utils/log.hpp"

int main()
{
    LOG_RAW("Normal log entry without newline");
    LOG("Normal log entry.");
    LOG_WARN("Warning log entry");
    LOG_ERROR("Error log entry");
    LOG_DEBUG("Debug log entry");

    int x = 5;
    LOG("\nA variable: ", LOG_DUMP(x));

    return 0;
}
