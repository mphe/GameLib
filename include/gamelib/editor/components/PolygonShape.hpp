#ifndef GAMELIB_POLYGON_SHAPE_HPP
#define GAMELIB_POLYGON_SHAPE_HPP

#include <SFML/Graphics.hpp>
#include "math/geometry/Polygon.hpp"
#include "gamelib/core/ecs/RenderComponent.hpp"
#include "gamelib/core/res/TextureResource.hpp"

namespace gamelib
{
    enum MappingMethod
    {
        MapWorld,
        MapLine,
        MapInstance,
        MapFit,
        MapStretch,
        NumMappingMethods
    };

    class PolygonShape : public RenderComponent
    {
        public:
            constexpr static const char* name = "PolygonShape";

        public:
            PolygonShape(size_t size = 10);
            virtual ~PolygonShape() {}

            virtual auto fetch(const math::Polygon<float>& pol) -> void;

            auto setTexOffset(float x, float y)       -> void;
            auto setTexOffset(const math::Vec2f& vec) -> void;
            auto getTexOffset()                       -> const math::Vec2f&;

            auto render(sf::RenderTarget& target) -> void;

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

        protected:
            virtual auto _mapTexture(const math::Polygon<float>& pol) -> void;

        public:
            int mappingMethod;
            TextureResource::Handle texture;

        protected:
            math::Vec2f _texoffset;
            sf::VertexArray _vertices;
    };
}

#endif
