#include "gamelib/components/geometry/AABB.hpp"
#include "gamelib/utils/conversions.hpp"

namespace gamelib
{
    AABB::AABB() :
        AABB(0, 0, 0, 0, 0)
    { }

    AABB::AABB(float x, float y, float w, float h, unsigned int flags) :
        AABB(math::AABBf(x, y, w, h), flags)
    { }

    AABB::AABB(const math::AABBf& aabb, unsigned int flags_) :
        CollisionComponent(name),
        _size(aabb.size),
        _rect(aabb)
    {
        flags = flags_;
        _setSupportedOps(true, true, false);
        _props.registerProperty("size", _size, PROP_METHOD(math::Vec2f, setSize), this);
    }


    void AABB::setSize(const math::Vec2f& size)
    {
        _size = size;
        _rect.size = _size * getScale();
        _markDirty();
    }

    void AABB::setSize(float w, float h)
    {
        setSize(math::Vec2f(w, h));
    }

    const math::AABBf& AABB::getBBox() const
    {
        return _rect;
    }

    void AABB::_onChanged(const sf::Transform& old)
    {
        _rect.pos = convert(getMatrix().transformPoint(0, 0));
        _rect.size = _size * getScale();
    }

    bool AABB::intersect(const math::Point2f& point) const
    {
        return _rect.contains(point);
    }

    Intersection AABB::intersect(const math::Line2f& line) const
    {
        return line.intersect(_rect);
    }

    Intersection AABB::intersect(const math::AABBf& rect) const
    {
        return _rect.intersect(rect);
    }

    Intersection AABB::sweep(const math::AABBf& rect, const math::Vec2f& vel) const
    {
        return rect.sweep(vel, _rect);
    }
}
