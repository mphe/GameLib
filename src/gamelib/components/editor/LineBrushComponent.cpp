#include "gamelib/components/editor/LineBrushComponent.hpp"
#include "gamelib/components/geometry/Polygon.hpp"
#include "gamelib/components/rendering/MeshRenderer.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/properties/PropComponent.hpp"

namespace gamelib
{
    LineBrushComponent::LineBrushComponent() :
        _linewidth(32)
    {
        auto cb = +[](const ComponentReference<PolygonCollider>* val, LineBrushComponent* self) {
            self->_line = *val;
            self->regenerate();
        };

        _props.registerProperty("linewidth", _linewidth, PROP_METHOD(_linewidth, setWidth), this);
        registerProperty(_props, "line", _line, *this, cb);
    }

    void LineBrushComponent::setWidth(int width)
    {
        if (_linewidth == width)
            return;

        _linewidth = width;
        regenerate();
    }

    int LineBrushComponent::getWidth() const
    {
        return _linewidth;
    }

    PolygonCollider* LineBrushComponent::getBrushPolygon() const
    {
        return _line.get();
    }

    void LineBrushComponent::regenerate() const
    {
        if (!_shape || !_pol || !_line || _line->size() == 0)
            return;

        math::Vec2f lastd, dir;
        bool start = true;

        auto cb = [&](const math::Line2f& seg) {
            float area = 0;
            auto norm = seg.d.normalized();

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
                if (lastd.normalized() == norm)
                    return false;

                area = (seg.p.x - p1.x) * (seg.p.y + p1.y)
                    + (p3.x - seg.p.x) * (p3.y + seg.p.y)
                    + (p1.x - p3.x) * (p1.y + p3.y);
            }

            dir = (norm + lastd.normalized()).right().normalized();
            dir *= (_linewidth / 2.0) / dir.angle_cos(lastd.right());

            _pol->add(seg.p - dir);
            _pol->add(seg.p + dir);

            // depending on clockwise or counter clockwise line direction,
            // different vertices have to be added.
            if (area != 0)
            {
                auto len = norm.right() * _linewidth;

                if (area > 0)   // clockwise
                {
                    _pol->add(seg.p - dir);
                    _pol->add(seg.p - dir + len);
                }
                else   // counter clockwise
                {
                    _pol->add(seg.p + dir - len);
                    _pol->add(seg.p + dir);
                }
            }

            lastd = seg.d;
            return false;
        };

        auto& linepol = _line->getLocal();
        _pol->clear();
        linepol.foreachSegment(cb);
        dir = lastd.right().normalized() * (_linewidth / 2.0);
        _pol->add(linepol.get(linepol.size() - 1) - dir);
        _pol->add(linepol.get(linepol.size() - 1) + dir);

        _shape->fetch(_pol->getLocal(), sf::TriangleStrip);
    }
}
