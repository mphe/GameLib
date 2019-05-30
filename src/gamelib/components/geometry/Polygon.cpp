#include "gamelib/components/geometry/Polygon.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/properties/PropDummy.hpp"
#include "math/geometry/intersect.hpp"

namespace gamelib
{
    constexpr const char* normaldir_hints[] = { "Both", "Left", "Right" };
    constexpr const char* filltype_hints[] = { "Open", "Closed", "Filled" };

    PolygonCollider::PolygonCollider(unsigned int flags_) :
        CollisionComponent(name),
        _global(_local),
        _filltype(_local.getFillType()),
        _normaldir(_local.getNormalDir())
    {
        auto normalsetter = +[](math::NormalDirection* var, const math::NormalDirection* val, PolygonCollider* self) {
            *var = *val;
            self->_local.setNormalDir(*val);
        };

        auto fillsetter = +[](math::FillType* var, const math::FillType* val, PolygonCollider* self) {
            *var = *val;
            self->_local.setFillType(*val);
        };

        flags = flags_;
        _local.setFillType(math::Closed);
        _local.setNormalDir(math::NormalLeft);
        _setSupportedOps(true, true, true);
        _props.registerProperty("Normals", _normaldir, normalsetter, this, 0, 3, normaldir_hints);
        _props.registerProperty("FillType", _filltype, fillsetter, this, 0, 3, filltype_hints);
        registerDummyProperty(_props, "vertices");
    }

    bool PolygonCollider::intersect(const math::Point2f& point) const
    {
        return math::intersect(point, _global);
    }

    Intersection PolygonCollider::intersect(const math::Line2f& line) const
    {
        return math::intersect(line, _global);
    }

    Intersection PolygonCollider::intersect(const math::AABBf& rect) const
    {
        Intersection isec;
        _global.foreachSegment([&](const math::Line2f& seg) {
                isec = math::intersect(seg, rect);
                return isec;
            });
        return isec;
    }

    Intersection PolygonCollider::sweep(const math::AABBf& rect, const math::Vec2f& vel) const
    {
        return math::sweep(rect, vel, _global);
    }

    void PolygonCollider::_onChanged(const sf::Transform& old)
    {
        _global.setMatrix(getMatrix());
    }

    math::AABBf PolygonCollider::getBBox() const
    {
        return _global.getBBox();
    }

    const math::AbstractPolygon<float>& PolygonCollider::getGlobal() const
    {
        return _global;
    }

    const math::AbstractPolygon<float>& PolygonCollider::getLocal() const
    {
        return _local;
    }

    bool PolygonCollider::loadFromJson(const Json::Value& node)
    {
        CollisionComponent::loadFromJson(node);
        _global.clear();

        if (node.isMember("vertices"))
        {
            auto& vertices = node["vertices"];
            math::Point2f p;
            for (auto& i : vertices)
            {
                if (gamelib::loadFromJson(i, p))
                    _local.add(p);
                else
                    LOG_WARN("Incorrect vertex format: ", node.toStyledString());
            }
        }
        return true;
    }

    void PolygonCollider::writeToJson(Json::Value& node) const
    {
        CollisionComponent::writeToJson(node);
        auto& vertices = node["vertices"];
        vertices.resize(_local.size());
        for (size_t i = 0; i < _local.size(); ++i)
            gamelib::writeToJson(vertices[Json::ArrayIndex(i)], _local.get(i));
    }

    void PolygonCollider::add(const math::Point2f& point, bool raw)
    {
        if (raw)
            _local.add(point);
        else
            _global.add(point);
        _markDirty();
    }

    void PolygonCollider::edit(size_t i, const math::Point2f& p, bool raw)
    {
        if (raw)
            _local.edit(i, p);
        else
            _global.edit(i, p);
        _markDirty();
    }

    void PolygonCollider::clear()
    {
        _local.clear();
        _markDirty();
    }

    size_t PolygonCollider::size() const
    {
        return _local.size();
    }
}
