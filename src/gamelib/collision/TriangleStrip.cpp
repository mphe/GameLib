#include "gamelib/collision/TriangleStrip.hpp"

namespace gamelib
{
    TriangleStrip::TriangleStrip(unsigned int flags, void* owner) :
        Collidable(flags, owner)
    { }

    bool TriangleStrip::intersect(const math::Point2f& point) const
    {
        return polygon.intersect(point);
    }

    Intersection TriangleStrip::intersect(const math::Line2f& line) const
    {
        return polygon.intersect(line);
    }

    Intersection TriangleStrip::intersect(const math::AABBf& rect) const
    {
        throw "Not implemented";
    }


    void TriangleStrip::move(float x, float y)
    {
        polygon.move(x, y);
    }

    void TriangleStrip::setPosition(float x, float y)
    {
        polygon.setOffset(x, y);
    }


    const math::Vec2f& TriangleStrip::getPosition() const
    {
        return polygon.getOffset();
    }

    const math::AABBf TriangleStrip::getBBox() const
    {
        return polygon.getBBox();
    }
}
