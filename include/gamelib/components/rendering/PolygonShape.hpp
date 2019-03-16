#ifndef GAMELIB_POLYGON_SHAPE_HPP
#define GAMELIB_POLYGON_SHAPE_HPP

#include <SFML/Graphics.hpp>
#include "math/geometry/PointSet.hpp"
#include "gamelib/components/RenderComponent.hpp"
#include "gamelib/core/res/TextureResource.hpp"

namespace gamelib
{
    constexpr const char* str_primitives[] = {
        "Points", "Lines", "LineStrip", "Triangles", "TriangleStrip", "TriangleFan", "Quads",
    };

    enum MappingMethod
    {
        MapWorld,
        MapLine,
        MapInstance,
        MapFit,     // unimplemented
        MapStretch, // unimplemented
        NumMappingMethods
    };

    constexpr const char* str_mappings[] = { "World", "Line", "Instance", "Fit", "Stretch" };

    class PolygonShape : public RenderComponent
    {
        public:
            constexpr static const char* name = "PolygonShape";

        public:
            PolygonShape();
            virtual ~PolygonShape() {}

            virtual auto fetch(const math::AbstractPointSet<float>& pol) -> void;
            virtual auto fetch(const math::AABBf& rect) -> void;

            auto adaptToTexture() -> void;

            auto setPrimitiveType(sf::PrimitiveType type) -> void;
            auto getPrimitiveType() const                 -> sf::PrimitiveType;

            auto setTexture(TextureResource::Handle tex) -> void;
            auto getTexture() const                      -> TextureResource::Handle;

            auto setTexOffset(const math::Vec2f& vec) -> void;
            auto getTexOffset() const                 -> const math::Vec2f&;

            auto setTexScale(const math::Vec2f& vec) -> void;
            auto getTexScale() const                 -> const math::Vec2f&;

            auto setMappingMethod(MappingMethod m) -> void;
            auto getMappingMethod() const          -> MappingMethod;

            auto render(sf::RenderTarget& target, sf::RenderStates states) const -> void;

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node) const  -> void;

        protected:
            auto _mapTexture() -> void;
            virtual auto _mapTexture(MappingMethod mappingMethod) -> void;

            // Adapt mapping on transform
            virtual auto _onChanged(const sf::Transform& old) -> void;

        private:
            TextureResource::Handle _tex;
            sf::PrimitiveType _primitiveType;
            math::Vec2f _texoffset;
            math::Vec2f _texscale;
            MappingMethod _mapping;
    };
}

#endif
