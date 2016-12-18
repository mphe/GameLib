#include "gamelib/collision/AABBCollision.hpp"

namespace gamelib
{
    AABBCollision::AABBCollision(float x, float y, float w, float h) :
        _rect(x, y, w, h)
    { }

    bool AABBCollision::contains(const math::Vec2f& point) const
    {
        return _rect.contains(point);
    }

    bool AABBCollision::intersects(const math::Line2f& line) const
    {

    }

    bool AABBCollision::intersects(const math::AABB<float>& rect) const
    {
        return _rect.intersects(rect);
    }

    math::AABB<float> AABBCollision::getBBox() const
    {
        return _rect;
    }

    void AABBCollision::move(float x, float y)
    {
        _rect.pos.x += x;
        _rect.pos.y += y;
    }

    void AABBCollision::setPosition(float x, float y)
    {
        _rect.pos.fill(x, y);
    }

    math::Vec2f AABBCollision::getPosition() const
    {
        return _rect.pos;
    }
}
