#include "gamelib/collision/AABBCollision.hpp"

namespace gamelib
{
    AABBCollision::AABBCollision(float x, float y, float w, float h) :
        _rect(x, y, w, h)
    { }

    bool AABBCollision::contains(const geometry::Vector2<float>& point) const
    {
        return _rect.contains(point);
    }

    bool AABBCollision::intersects(const geometry::AABB<float>& rect) const
    {
        return _rect.intersects(rect);
    }

    geometry::AABB<float> AABBCollision::getBBox() const
    {
        return _rect;
    }
}
