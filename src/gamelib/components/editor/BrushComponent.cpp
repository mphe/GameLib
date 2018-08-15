#include "gamelib/components/editor/BrushComponent.hpp"
#include "gamelib/components/rendering/PolygonShape.hpp"
#include "gamelib/components/geometry/Polygon.hpp"
#include "gamelib/core/ecs/Entity.hpp"

namespace gamelib
{
    constexpr const char* BrushComponent::name;

    BrushComponent::BrushComponent() :
        Identifier(name),
        _pol(nullptr),
        _shape(nullptr)
    { }

    void BrushComponent::add(const math::Point2f& p) const
    {
        if (getBrushPolygon()->size() == 0)
            getBrushPolygon()->setPosition(p);
        getBrushPolygon()->add(p);
        regenerate();
    }

    void BrushComponent::edit(size_t i, const math::Point2f& p) const
    {
        getBrushPolygon()->edit(i, p);
        regenerate();
    }

    void BrushComponent::remove(size_t i) const
    {
        // TODO
    }

    Polygon* BrushComponent::getBrushPolygon() const
    {
        return _pol;
    }

    PolygonShape* BrushComponent::getBrushShape() const
    {
        return _shape;
    }

    void BrushComponent::_refresh()
    {
        _shape = getEntity()->findByName<PolygonShape>();
        _pol = nullptr;

        getEntity()->findAllByName<Polygon>([&](Polygon* pol) {
                if (pol->getPolygon().type == math::TriangleStrip)
                {
                    if (_pol != pol)
                    {
                        _pol = pol;
                        regenerate();
                    }
                    return true;
                }
                return false;
            });
    }

    void BrushComponent::regenerate() const
    {
        if (_shape && _pol)
            _shape->fetch(_pol->getPolygon());
    }
}
