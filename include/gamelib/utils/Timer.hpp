#ifndef GAMELIB_TIMER_HPP
#define GAMELIB_TIMER_HPP

#include "gamelib/core/update/Updatable.hpp"

namespace gamelib
{
    // Countdown from a specified number of seconds. When time is up,
    // isFinished() returns true. The user must manually call stop() to
    // shift from "finished" state to "stopped" state.
    class Timer : public Updatable
    {
        public:
            Timer();

            // Set default time
            Timer(float seconds);

            // Start with the given time and optionally set it as default time
            auto start(float seconds, bool setDefault = false) -> void;

            // Start with default time
            auto start() -> void;

            // Stop timer. Changes from running or finished state to stopped state.
            auto stop()  -> void;

            // Get time left in percent. Can be inaccurate if current
            // countdown time is not set as default time.
            auto getPercent() const -> float;

            // Get remaining time in seconds
            auto getRemaining() const -> float;

            auto isFinished() const   -> bool;
            auto isRunning() const    -> bool;
            auto isStopped() const    -> bool;

            auto update(float elapsed) -> void final;

            // Returns isFinished().
            explicit operator bool() const;

        public:
            float seconds; // Default time

        private:
            float _left;
    };
}

#endif
