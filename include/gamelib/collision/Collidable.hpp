#ifndef GAMELIB_COLLISION_COLLIDABLE
#define GAMELIB_COLLISION_COLLIDABLE

#include "math/geometry/Vector2.hpp"
#include "math/geometry/AABB.hpp"
#include "gamelib/IdentifiableBase.hpp"

namespace sf
{
    class RenderTarget;
}

namespace gamelib
{
    class Collidable : virtual public IdentifiableBase
    {
        public:
            virtual ~Collidable() {};

            virtual auto contains(const geometry::Vector2<float>& point) const  -> bool = 0;
            virtual auto intersects(const geometry::AABB<float>& rect) const    -> bool = 0;
            virtual auto getBBox() const                                        -> geometry::AABB<float> = 0;
    };
}

#endif
