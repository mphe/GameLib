#ifndef GAMELIB_AABB_COLLISON_HPP
#define GAMELIB_AABB_COLLISON_HPP

#include "Collidable.hpp"

namespace gamelib
{
    class AABBCollision : public Collidable
    {
        public:
            AABBCollision(float x, float y, float w, float h, unsigned int flags = 0);
            virtual ~AABBCollision() {}

            auto intersect(const math::Point2f& point) const -> bool;
            auto intersect(const math::Line2f& line) const -> Intersection;
            auto intersect(const math::AABBf& rect) const -> Intersection;

            auto move(float x, float y)         -> void;
            auto setPosition(float x, float y)  -> void;

            auto getPosition() const    -> const math::Vec2f&;
            auto getBBox() const        -> const math::AABBf;

        protected:
            math::AABBf _rect;
    };
}

#endif
