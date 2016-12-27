#ifndef GAMELIB_COLLISION_COLLIDABLE
#define GAMELIB_COLLISION_COLLIDABLE

#include "math/geometry/Vector.hpp"
#include "math/geometry/Line2.hpp"
#include "math/geometry/AABB.hpp"

/*
 * Base class for objects that have a "body" and can collide with others.
 * A collidable object can be moved to a different position, has a
 * bounding box and can intersect at least with points and lines.
 * Additionally, there are 2 fields "flags" and "owner".
 * "flags" can be used for various collision related flags,
 * e.g. solid, kill, hurt, ... .
 * "owner" is useful in entity-component like systems.
 */

namespace gamelib
{
    typedef math::Intersection<float> Intersection;

    class Collidable
    {
        public:
            Collidable() : flags(0), owner(nullptr) {};
            Collidable(unsigned int flags_, void* owner_) :
                flags(flags_), owner(owner_) {};
            virtual ~Collidable() {};

            virtual auto intersect(const math::Point2f& point) const -> bool = 0;
            virtual auto intersect(const math::Line2f& line) const   -> Intersection = 0;
            virtual auto intersect(const math::AABBf& rect) const    -> Intersection = 0;

            virtual auto move(float x, float y)         -> void = 0;
            virtual auto setPosition(float x, float y)  -> void = 0;

            virtual auto getPosition() const    -> const math::Vec2f& = 0;
            virtual auto getBBox() const        -> const math::AABBf = 0;

        public:
            unsigned int flags;
            void* owner;
    };
}

#endif
