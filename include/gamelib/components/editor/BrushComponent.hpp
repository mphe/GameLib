#ifndef GAMELIB__BRUSH_COMPONENT_HPP
#define GAMELIB__BRUSH_COMPONENT_HPP

#include "gamelib/core/Identifier.hpp"
#include "gamelib/core/ecs/Component.hpp"
#include "math/geometry/Point2.hpp"

namespace gamelib
{
    class PolygonShape;
    class Polygon;

    class BrushComponent : public Identifier<0xaece07e3, Component>
    {
        public:
            constexpr static const char* name = "BrushComponent";

        public:
            BrushComponent();
            BrushComponent(const std::string& name);
            virtual ~BrushComponent() {};

            auto add(const math::Point2f& p) const            -> void;
            auto edit(size_t i, const math::Point2f& p) const -> void;
            auto remove(size_t i) const                       -> void;

            // Not very ECSy but saves a bunch of lines and makes working with
            // brushes easier. It's only intended to be used in the editor anyways.
            virtual auto getBrushPolygon() const -> Polygon*;
            virtual auto getBrushShape() const   -> PolygonShape*;

            virtual auto regenerate() const -> void;

        protected:
            virtual auto _refresh() -> void;

        protected:
            Polygon* _pol;
            PolygonShape* _shape;
    };
}

#endif
