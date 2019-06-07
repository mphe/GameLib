#include "gamelib/utils/Timer.hpp"

namespace gamelib
{
    constexpr const float timer_finished = 0;
    constexpr const float timer_stopped = -1;

    Timer::Timer() :
        seconds(timer_stopped),
        _left(timer_stopped)
    { }

    Timer::Timer(float seconds_) :
        seconds(seconds_),
        _left(timer_stopped)
    { }

    auto Timer::start(float seconds) -> void
    {
        this->seconds = seconds;
        _left = seconds;
    }

    auto Timer::start() -> void
    {
        _left = seconds;
    }

    auto Timer::stop() -> void
    {
        _left = timer_stopped;
    }

    auto Timer::getRemaining() const -> float
    {
        return _left;
    }

    auto Timer::isFinished() const -> bool
    {
        return _left == timer_finished;
    }

    auto Timer::isRunning() const -> bool
    {
        return _left > 0;
    }

    auto Timer::isStopped() const -> bool
    {
        return _left == timer_stopped;
    }

    auto Timer::update(float elapsed) -> void
    {
        if (!isRunning())
            return;

        _left -= elapsed;
        if (_left <= 0)
            _left = timer_finished;
    }

    Timer::operator bool() const
    {
        return isFinished();
    }
}
