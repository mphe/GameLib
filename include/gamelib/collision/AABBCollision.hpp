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

            bool contains(const geometry::Vector2<float>& point) const;
            bool intersects(const geometry::AABB<float>& rect) const;
            geometry::AABB<float> getBBox() const;

        protected:
            geometry::AABB<float> _rect;
    };
}

#endif
