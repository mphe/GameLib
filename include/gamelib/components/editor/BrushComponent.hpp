#ifndef EDITOR_BRUSHCOMPONENT_HPP
#define EDITOR_BRUSHCOMPONENT_HPP

#include "gamelib/core/Identifier.hpp"
#include "gamelib/core/ecs/Component.hpp"
#include "gamelib/components/geometry/Polygon.hpp"
#include "math/geometry/Point2.hpp"

namespace gamelib
{
    class PolygonShape;

    class BrushComponent : public Identifier<0xaece07e3, Component>
    {
        public:
            constexpr static const char* name = "BrushComponent";

        public:
            BrushComponent();
            virtual ~BrushComponent() {};

            auto add(const math::Point2f& p) const            -> void;
            auto edit(size_t i, const math::Point2f& p) const -> void;
            auto remove(size_t i) const                       -> void;

            // Not very ECSy but saves a bunch of lines and makes working with
            // brushes easier. It's only intended to be used in the editor anyways.
            auto getBrushPolygon() const -> math::Polygon<float>*;
            auto getBrushShape() const   -> PolygonShape*;

            auto setWidth(int width) -> void;
            auto getWidth() const    -> int;

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

        protected:
            virtual auto _refresh()  -> void;

            auto _regenerate() const -> void;

        protected:
            Polygon* _line;
            math::Polygon<float>* _pol;
            PolygonShape* _shape;
            int _linewidth;
    };
}

#endif
