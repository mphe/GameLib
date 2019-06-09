#ifndef GAMELIB_TIMER_HPP
#define GAMELIB_TIMER_HPP

#include "gamelib/core/update/Updatable.hpp"

// Countdown from a specified number of seconds.
// When time is up, isFinished() returns true.
// The user must manually call stop() to shift from
// "finished" state to "stopped" state.

namespace gamelib
{
    class Timer : public Updatable
    {
        public:
            Timer();
            Timer(float seconds);

            auto start(float seconds)  -> void;
            auto start()               -> void;
            auto stop()                -> void;
            auto getRemaining() const  -> float;
            auto getPercent() const    -> float;
            auto isFinished() const    -> bool;
            auto isRunning() const     -> bool;
            auto isStopped() const     -> bool;
            auto update(float elapsed) -> void final override;

            explicit operator bool() const;

        public:
            float seconds;

        private:
            float _left;
    };
}

#endif
