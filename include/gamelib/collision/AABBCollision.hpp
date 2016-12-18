#ifndef GAMELIB_AABB_COLLISON_HPP
#define GAMELIB_AABB_COLLISON_HPP

#include "Collidable.hpp"

namespace gamelib
{
    class AABBCollision : public Collidable
    {
        public:
            AABBCollision(float x, float y, float w, float h);
            virtual ~AABBCollision() {}

            bool contains(const math::Vec2f& point) const;
            bool intersects(const math::AABBf& rect) const;
            math::AABBf getBBox() const;

        protected:
            math::AABBf _rect;
    };
}

#endif
