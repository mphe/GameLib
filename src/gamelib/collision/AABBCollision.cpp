#include "gamelib/collision/AABBCollision.hpp"

namespace gamelib
{
    AABBCollision::AABBCollision(float x, float y, float w, float h) :
        _rect(x, y, w, h)
    { }

    bool AABBCollision::contains(const math::Vec2f& point) const
    {
        return _rect.contains(point.asPoint());
    }

    bool AABBCollision::intersects(const math::AABB<float>& rect) const
    {
        return _rect.intersect(rect);
    }

    math::AABB<float> AABBCollision::getBBox() const
    {
        return _rect;
    }
}
