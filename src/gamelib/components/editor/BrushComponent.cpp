#include "gamelib/components/editor/BrushComponent.hpp"
#include "gamelib/components/rendering/PolygonShape.hpp"
#include "gamelib/components/geometry/Polygon.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/properties/PropComponent.hpp"

namespace gamelib
{
    constexpr const char* BrushComponent::name;

    BrushComponent::BrushComponent() :
        BrushComponent(name)
    { }

    BrushComponent::BrushComponent(const std::string& name_) :
        Identifier(name_),
        _pol(nullptr),
        _shape(nullptr)
    {
        auto cb = +[](Polygon** var, Polygon* const* val, BrushComponent* self) {
            *var = *val;
            self->regenerate();
        };

        registerProperty(_props, "polygon", _pol, *this, cb);
        registerProperty(_props, "shape", _shape, *this, (ComponentPropSetter<PolygonShape, BrushComponent>)cb);
    }

    void BrushComponent::add(const math::Point2f& p, bool raw) const
    {
        getBrushPolygon()->add(p, raw);
        regenerate();
    }

    void BrushComponent::edit(size_t i, const math::Point2f& p, bool raw) const
    {
        getBrushPolygon()->edit(i, p, raw);
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
        // _shape = getEntity()->findByName<PolygonShape>();
        // _pol = nullptr;
        //
        // getEntity()->findAllByName<Polygon>([&](Polygon* pol) {
        //         if (pol->getPolygon().type == math::TriangleStrip)
        //         {
        //             if (_pol != pol)
        //             {
        //                 _pol = pol;
        //                 regenerate();
        //             }
        //             return true;
        //         }
        //         return false;
        //     });
    }

    void BrushComponent::regenerate() const
    {
        if (_shape && _pol)
            _shape->fetch(_pol->getPolygon());
    }
}
