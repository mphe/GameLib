#ifndef GAMELIB_POLYGON_SHAPE_HPP
#define GAMELIB_POLYGON_SHAPE_HPP

#include <SFML/Graphics.hpp>
#include "math/geometry/Polygon.hpp"
#include "gamelib/components/RenderComponent.hpp"
#include "gamelib/core/res/TextureResource.hpp"

// This seems to become a blob class for all rendering related stuff.
// Might be not so good...

namespace gamelib
{
    enum MappingMethod
    {
        MapWorld,
        MapLine,
        MapInstance,
        MapFit,     // unimplemented
        MapStretch, // unimplemented
        NumMappingMethods
    };

    constexpr const char* mapping_strings[] = { "World", "Line", "Instance", "Fit", "Stretch" };

    class PolygonShape : public RenderComponent
    {
        public:
            constexpr static const char* name = "PolygonShape";

        public:
            PolygonShape();
            virtual ~PolygonShape() {}

            virtual auto fetch(const math::Polygon<float>& pol, MappingMethod mappingMethod = MapWorld) -> void;

            // Maybe rename this
            // Creates a rect of the given size and uses the position as texture offset
            virtual auto fetch(const math::AABBf& rect, MappingMethod mappingMethod = MapWorld) -> void;

            auto adaptToTexture() -> void;

            auto setTexOffset(float x, float y)       -> void;
            auto setTexOffset(const math::Vec2f& vec) -> void;
            auto getTexOffset()                       -> const math::Vec2f&;

            auto size() const -> size_t;

            auto render(sf::RenderTarget& target, sf::RenderStates states) const -> void;

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

        protected:
            virtual auto _mapTexture(MappingMethod mappingMethod) -> void;

        public:
            TextureResource::Handle texture;

        protected:
            math::Vec2f _texoffset;
    };
}

#endif
