#include "gamelib/components/editor/BrushComponent.hpp"
#include "gamelib/components/rendering/PolygonShape.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/ecs/CollisionComponent.hpp"
#include "gamelib/core/geometry/flags.hpp"

namespace gamelib
{
    constexpr const char* BrushComponent::name; // Why?

    BrushComponent::BrushComponent() :
        Identifier(name),
        _line(nullptr),
        _pol(nullptr),
        _shape(nullptr)
    { }

    void BrushComponent::add(const math::Point2f& p) const
    {
        if (getBrushPolygon()->size() == 0)
            getBrushPolygon()->setOffset(p.asVector());
        getBrushPolygon()->add(p);
        // getEntity()->getTransform().refreshBBox();
        _regenerate();
    }

    void BrushComponent::edit(size_t i, const math::Point2f& p) const
    {
        getBrushPolygon()->edit(i, p);
        // getEntity()->getTransform().refreshBBox();
        _regenerate();
    }

    void BrushComponent::remove(size_t i) const
    {
        // TODO: implement for lines
    }

    math::Polygon<float>* BrushComponent::getBrushPolygon() const
    {
        return _line ? &_line->polygon : _pol;
    }

    PolygonShape* BrushComponent::getBrushShape() const
    {
        return _shape;
    }

    void BrushComponent::setWidth(int width)
    {
        _linewidth = width;
        if (_line)
            _regenerate();
    }

    int BrushComponent::getWidth() const
    {
        return _linewidth;
    }

    bool BrushComponent::loadFromJson(const Json::Value& node)
    {
        Component::loadFromJson(node);
        int w = node.get("linewidth", _linewidth).asInt();
        setWidth(w);
        return true;
    }

    void BrushComponent::writeToJson(Json::Value& node)
    {
        Component::writeToJson(node);
        node["linewidth"] = _linewidth;
    }

    void BrushComponent::_refresh()
    {
        getEntity()->findAllByType<CollisionComponent>([&](CollisionComponent* comp) {
                if (comp->getName() == Polygon::name)
                {
                    auto pol = static_cast<Polygon*>(comp);
                    if (pol->polygon.type == math::TriangleStrip)
                        _pol = &pol->polygon;
                    else if (pol->polygon.type == math::LineStrip)
                        _line = pol;
                }
                return false;
            });

        _shape = getEntity()->findByType<PolygonShape>();
    }

    void BrushComponent::_regenerate() const
    {
        if (!_line)
        {
            _shape->fetch(*_pol);
            return;
        }

        if (_line->polygon.size() == 0)
            return;

        auto& linepol = _line->polygon;
        math::Vec2f lastd, dir;
        bool start = true;
        _pol->clear();

        linepol.foreachSegment([&](const math::Line2f& seg) {
            float area = 0;
            if (start)
            {
                start = false;
                lastd = seg.d;
            }
            else
            {
                // Compute if the line turns clockwise or counter clockwise
                // Total hours wasted here because I didn't google the
                // problem / didn't knew what exactly to google for: 11
                auto p1 = seg.p - lastd;
                auto p3 = seg.p + seg.d;

                // Break if the direction hasn't changed
                if (lastd.normalized() == seg.d.normalized())
                    return false;

                area = (seg.p.x - p1.x) * (seg.p.y + p1.y)
                        + (p3.x - seg.p.x) * (p3.y + seg.p.y)
                        + (p1.x - p3.x) * (p1.y + p3.y);
            }

            dir = (seg.d.normalized() + lastd.normalized()).right().normalized();// / 2;
            dir *= (_linewidth / 2.0) / dir.angle_cos(lastd.right());

            _pol->add(seg.p - dir);
            _pol->add(seg.p + dir);

            // depending on clockwise or counter clockwise line direction,
            // different vertices have to be added.
            if (area != 0)
            {
                if (area > 0)   // clockwise
                {
                    _pol->add(seg.p - dir);
                    _pol->add(seg.p - dir + seg.d.normalized().right() * _linewidth);
                }
                else   // counter clockwise
                {
                    _pol->add(seg.p + dir - seg.d.normalized().right() * _linewidth);
                    _pol->add(seg.p + dir);
                }
            }

            lastd = seg.d;
            return false;
        });

        dir = lastd.right().normalized() * (_linewidth / 2.0);
        _pol->add(linepol.get(-1) - dir);
        _pol->add(linepol.get(-1) + dir);

        _shape->fetch(*_pol, MapLine);
    }
}
