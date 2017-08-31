#include "gamelib/geometry/AABB.hpp"

namespace gamelib
{
    AABBCollision::AABBCollision(float x, float y, float w, float h, unsigned int flags) :
        AABBCollision(math::AABBf(x, y, w, h), flags)
    { }

    AABBCollision::AABBCollision(const math::AABBf& aabb, unsigned int flags_) :
        _scale(1, 1),
        _rect(aabb)
    {
        flags = flags_;
        _supported = movable | scalable;
    }


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


    void AABBCollision::move(const math::Vec2f& rel)
    {
        _rect.pos += rel;
    }

    void AABBCollision::scale(const math::Vec2f& scale)
    {
        _rect.size *= scale;
        _scale *= scale;
    }

    const math::Point2f& AABBCollision::getPosition() const
    {
        return _rect.pos.asPoint();
    }

    const math::Vec2f& AABBCollision::getScale() const
    {
        return _scale;
    }

    const math::AABBf& AABBCollision::getBBox() const
    {
        return _rect;
    }
}
