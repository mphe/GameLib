#ifndef GAMELIB_COLLISION_COLLIDABLE_HPP
#define GAMELIB_COLLISION_COLLIDABLE_HPP

#include "Transformable.hpp"
#include "math/geometry/Line2.hpp"

/*
 * TODO: Consider removing this abstraction and put it directly inside
 *       CollisionComponent.
 *
 * Base class for objects that have a "body" and can collide with others.
 * A collidable object can be moved to a different position, has a
 * bounding box and can intersect at least with points and lines.
 */

namespace gamelib
{
    typedef math::Intersection<float> Intersection;

    class Collidable : public Transformable
    {
        public:
            Collidable() : flags(0) {};
            Collidable(unsigned int flags_) : flags(flags_) {};
            virtual ~Collidable() {};

            virtual auto intersect(const math::Point2f& point) const -> bool = 0;
            virtual auto intersect(const math::Line2f& line) const   -> Intersection = 0;
            virtual auto intersect(const math::AABBf& rect) const    -> Intersection = 0;

            virtual auto sweep(const math::AABBf& rect, const math::Vec2f& vel) const -> Intersection = 0;

        public:
            unsigned int flags;
    };
}

#endif
