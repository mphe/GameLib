#ifndef GAMELIB_ACCELERATION_HPP
#define GAMELIB_ACCELERATION_HPP

#include <limits>
#include "math/geometry/Vector2.hpp"

namespace gamelib
{
    class Acceleration
    {
        public:
            Acceleration();
            Acceleration(float accl, float dir = 270, float min = 0, float max = std::numeric_limits<float>::max());

            void update(float elapsed);

            // Set speed to min
            void reset();

            // Set speed to 0
            void stop();

            geometry::Vector2<float> getVector() const;

        public:
            float min;
            float max;
            float accl;
            float speed;
            float dir;
    };
}

#endif
