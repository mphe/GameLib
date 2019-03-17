#ifndef GAMELIB_POLYGON_BRUSH_COMPONENT_HPP
#define GAMELIB_POLYGON_BRUSH_COMPONENT_HPP

#include "gamelib/core/Identifier.hpp"
#include "gamelib/core/ecs/Component.hpp"
#include "math/geometry/Point2.hpp"

namespace gamelib
{
    class PolygonShape;
    class PolygonCollider;

    class PolygonBrushComponent : public Identifier<0xaece07e3, Component>
    {
        public:
            constexpr static const char* name = "PolygonBrushComponent";

        public:
            PolygonBrushComponent();
            PolygonBrushComponent(const std::string& name);
            virtual ~PolygonBrushComponent() {};

            auto add(const math::Point2f& p, bool raw = true) const            -> void;
            auto edit(size_t i, const math::Point2f& p, bool raw = true) const -> void;
            auto remove(size_t i) const                       -> void;

            // Not very ECSy but saves a bunch of lines and makes working with
            // brushes easier. It's only intended to be used in the editor anyways.
            virtual auto getBrushPolygon() const -> PolygonCollider*;
            virtual auto getBrushShape() const   -> PolygonShape*;

            virtual auto regenerate() const -> void;

        protected:
            PolygonCollider* _pol;
            PolygonShape* _shape;
    };
}

#endif
