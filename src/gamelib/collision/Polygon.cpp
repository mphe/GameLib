#include "gamelib/collision/Polygon.hpp"

namespace gamelib
{
    Polygon::Polygon() :
        Polygon(math::LineStrip)
    { }

    Polygon::Polygon(math::PolygonType type, unsigned int flags, void* owner) :
        Collidable(flags, owner),
        polygon(type)
    { }

    bool Polygon::intersect(const math::Point2f& point) const
    {
        return polygon.intersect(point);
    }

    Intersection Polygon::intersect(const math::Line2f& line) const
    {
        return polygon.intersect(line);
    }

    Intersection Polygon::intersect(const math::AABBf& rect) const
    {
        throw "Not implemented";
    }


    void Polygon::move(float x, float y)
    {
        polygon.move(x, y);
    }

    void Polygon::setPosition(float x, float y)
    {
        polygon.setOffset(x, y);
    }

    const math::Vec2f& Polygon::getPosition() const
    {
        return polygon.getOffset();
    }

    const math::AABBf Polygon::getBBox() const
    {
        return polygon.getBBox();
    }
}
