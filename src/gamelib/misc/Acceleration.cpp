#include "gamelib/misc/Acceleration.hpp"
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

    void Acceleration::update(float fps)
    {
        // TODO: Frame independency works for any constant framerate, e.g. 10, 30, 60, 100, ...
        // Not tested with varying/unstable framerates yet.
        speed = std::min(speed + accl / fps, max);
    }

    geometry::Vector2<float> Acceleration::getVector() const
    {
        return geometry::Vector2<float>::fromDirection(speed, dir);
    }

    void Acceleration::reset()
    {
        speed = min;
    }
}
