#include "gamelib/components/geometry/AABB.hpp"
#include "gamelib/utils/json.hpp"

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
        _scale(1, 1),
        _rect(aabb)
    {
        flags = flags_;
        _supported = movable | scalable;
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


    void AABB::move(const math::Vec2f& rel)
    {
        _rect.pos += rel;
    }

    void AABB::scale(const math::Vec2f& scale)
    {
        _rect.size *= scale;
        _scale *= scale;
    }

    const math::Point2f& AABB::getPosition() const
    {
        return _rect.pos.asPoint();
    }

    const math::Vec2f& AABB::getScale() const
    {
        return _scale;
    }

    const math::AABBf& AABB::getBBox() const
    {
        return _rect;
    }

    bool AABB::loadFromJson(const Json::Value& node)
    {
        CollisionComponent::loadFromJson(node);
        // gamelib::loadFromJson(node["pos"], _rect.pos);
        gamelib::loadFromJson(node["size"], _rect.size);
        return true;
    }

    void AABB::writeToJson(Json::Value& node)
    {
        CollisionComponent::writeToJson(node);
        // gamelib::writeToJson(node["pos"], _rect.pos);
        gamelib::writeToJson(node["size"], _rect.size);
    }
}
