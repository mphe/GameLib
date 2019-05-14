#include "gamelib/components/editor/PolygonBrushComponent.hpp"
#include "gamelib/components/rendering/MeshRenderer.hpp"
#include "gamelib/components/geometry/Polygon.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/properties/PropComponent.hpp"

namespace gamelib
{
    constexpr const char* PolygonBrushComponent::name;

    PolygonBrushComponent::PolygonBrushComponent() :
        PolygonBrushComponent(name)
    { }

    PolygonBrushComponent::PolygonBrushComponent(const std::string& name_) :
        Identifier(name_),
        _pol(nullptr),
        _shape(nullptr)
    {
        auto cb = +[](PolygonCollider** var, PolygonCollider* const* val, PolygonBrushComponent* self) {
            *var = *val;
            self->regenerate();
        };

        registerProperty(_props, "polygon", _pol, *this, cb);
        registerProperty(_props, "shape", _shape, *this, (ComponentPropSetter<MeshRenderer, PolygonBrushComponent>)cb);
    }

    void PolygonBrushComponent::add(const math::Point2f& p, bool raw) const
    {
        getBrushPolygon()->add(p, raw);
        regenerate();
    }

    void PolygonBrushComponent::edit(size_t i, const math::Point2f& p, bool raw) const
    {
        getBrushPolygon()->edit(i, p, raw);
        regenerate();
    }

    void PolygonBrushComponent::remove(size_t i) const
    {
        // TODO
    }

    PolygonCollider* PolygonBrushComponent::getBrushPolygon() const
    {
        return _pol;
    }

    MeshRenderer* PolygonBrushComponent::getBrushShape() const
    {
        return _shape;
    }

    void PolygonBrushComponent::regenerate() const
    {
        if (_shape && _pol)
            _shape->fetch(_pol->getLocal(), sf::TriangleFan);
    }
}
