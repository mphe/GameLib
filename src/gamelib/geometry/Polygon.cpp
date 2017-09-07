#include "gamelib/geometry/Polygon.hpp"
#include "gamelib/utils/json.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    Polygon::Polygon(unsigned int flags) :
        Polygon(math::TriangleStrip, flags)
    { }

    Polygon::Polygon(math::PolygonType type, unsigned int flags_) :
        CollisionComponent(name),
        polygon(type)
    {
        flags = flags_;
        _supported = movable | scalable; // TODO: rotation
    }

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


    void Polygon::move(const math::Vec2f& rel)
    {
        polygon.move(rel);
    }

    void Polygon::scale(const math::Vec2f& scale)
    {
        polygon.setScale(getScale() * scale);
    }


    const math::Point2f& Polygon::getPosition() const
    {
        return polygon.getOffset().asPoint();
    }

    const math::Vec2f& Polygon::getScale() const
    {
        return polygon.getScale();
    }

    const math::AABBf& Polygon::getBBox() const
    {
        return polygon.getBBox();
    }


    bool Polygon::loadFromJson(const Json::Value& node)
    {
        CollisionComponent::loadFromJson(node);
        polygon.clear();
        polygon.type = static_cast<math::PolygonType>(node.get("type", math::TriangleStrip).asInt());
        if (node.isMember("vertices"))
        {
            auto& vertices = node["vertices"];
            math::Point2f p;
            for (auto& i : vertices)
            {
                if (gamelib::loadFromJson(i, p))
                    polygon.addRaw(p);
                else
                    LOG_WARN("Incorrect vertex format: ", node.toStyledString());
            }
        }
        return true;
    }

    void Polygon::writeToJson(Json::Value& node)
    {
        CollisionComponent::writeToJson(node);
        node["type"] = polygon.type;
        auto& vertices = node["vertices"];
        vertices.resize(polygon.size());
        for (int i = 0; i < polygon.size(); ++i)
            gamelib::writeToJson(vertices[i], polygon.getRaw(i));
    }

}
