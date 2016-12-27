#include "gamelib/collision/AABBCollision.hpp"

namespace gamelib
{
    AABBCollision::AABBCollision(float x, float y, float w, float h, unsigned int flags, void* owner) :
        Collidable(flags, owner),
        _rect(x, y, w, h)
    { }

    bool AABBCollision::intersect(const math::Point2f& point) const
    {
        return _rect.contains(point);
    }

    Intersection AABBCollision::intersect(const math::Line2f& line) const
    {
        return line.intersect(_rect);
    }

    Intersection AABBCollision::intersect(const math::AABBf& rect) const
    {
        return rect.intersect(_rect);
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

    const math::Vec2f& AABBCollision::getPosition() const
    {
        return _rect.pos;
    }

    const math::AABBf AABBCollision::getBBox() const
    {
        return _rect;
    }
}
