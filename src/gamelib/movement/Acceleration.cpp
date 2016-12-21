#include "gamelib/movement/Acceleration.hpp"
#include <algorithm> // std::min

namespace gamelib
{
    Acceleration::Acceleration() :
        Acceleration(0)
    { }

    Acceleration::Acceleration(float accl, float dir, float min, float max) :
        min(min),
        max(max),
        accl(accl),
        speed(min),
        dir(dir)
    { }

    void Acceleration::update(float elapsed)
    {
        // TODO: Frame independency works for any constant framerate, e.g. 10, 30, 60, 100, ...
        // Not tested with varying/unstable framerates yet.
        if (accl > 0)
            speed = std::min(speed + accl * elapsed, max);
        else
            speed = std::max(speed + accl * elapsed, max);
    }

    math::Vec2f Acceleration::getVector() const
    {
        return math::Vec2f::fromDirection(speed, dir);
    }

    void Acceleration::reset()
    {
        speed = min;
    }

    void Acceleration::stop()
    {
        speed = 0;
    }
}
