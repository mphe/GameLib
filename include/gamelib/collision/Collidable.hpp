#ifndef GAMELIB_COLLISION_COLLIDABLE
#define GAMELIB_COLLISION_COLLIDABLE

#include "math/geometry/Vector.hpp"
#include "math/geometry/AABB.hpp"

namespace gamelib
{
    class Collidable
    {
        public:
            virtual ~Collidable() {};

            virtual auto contains(const math::Vec2f& point) const  -> bool = 0;
            virtual auto intersects(const math::AABBf& rect) const -> bool = 0;
            virtual auto getBBox() const                           -> math::AABBf = 0;
    };
}

#endif
