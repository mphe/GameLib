#ifndef GAMELIB_PHYSICAL_HPP
#define GAMELIB_PHYSICAL_HPP

#include "math/geometry/Vector2.hpp"

/*
 * Physical is a simple interface defining movement functions.
 */

namespace gamelib
{
    class Physical
    {
        public:
            virtual ~Physical() {};

            // Move relative to the current coordinates ignoring collisions
            virtual void move(float x, float y) = 0;

            // Move relative to the current coordinates and stop if a collision occurs
            virtual void moveContact(float x, float y) = 0;

            virtual bool placeFree(float x, float y) const = 0;
            virtual bool placeFreeRelative(float x, float y) const = 0;

            virtual geometry::Vector2f getPosition() const = 0;
    };
}

#endif
