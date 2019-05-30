#ifndef GAMELIB_MESHRENDERER_HPP
#define GAMELIB_MESHRENDERER_HPP

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
        MapInstance,
        MapLine,
        // MapFit,     // unimplemented
        // MapStretch, // unimplemented
        NumMappingMethods
    };

    constexpr const char* str_mappings[] = { "World", "Instance", "Line", /* "Fit", "Stretch", */ };

    class MeshRenderer : public RenderComponent
    {
        public:
            constexpr static const char* name = "MeshRenderer";

        public:
            MeshRenderer();
            virtual ~MeshRenderer() {}

            virtual auto fetch(const math::AbstractPointSet<float>& pol, sf::PrimitiveType type)  -> void;
            virtual auto fetch(const sf::Vector2f* vertices, size_t size, sf::PrimitiveType type) -> void;
            virtual auto fetch(const math::AABBf& rect, sf::PrimitiveType type = sf::Quads)       -> void;

            auto setPrimitiveType(sf::PrimitiveType type) -> void;
            auto getPrimitiveType() const                 -> sf::PrimitiveType;
            auto setTexture(TextureResource::Handle tex)  -> void;
            auto getTexture() const                       -> TextureResource::Handle;
            auto setTexOffset(const math::Vec2f& vec)     -> void;
            auto getTexOffset() const                     -> const math::Vec2f&;
            auto setTexScale(const math::Vec2f& vec)      -> void;
            auto getTexScale() const                      -> const math::Vec2f&;
            auto setMappingMethod(MappingMethod m)        -> void;
            auto getMappingMethod() const                 -> MappingMethod;

            auto size() const -> size_t;

            auto loadFromJson(const Json::Value& node) -> bool final override;
            auto writeToJson(Json::Value& node) const  -> void final override;

        protected:
            virtual auto _mapTexture(MappingMethod mappingMethod) -> void;
            auto _mapTexture()                                    -> void;
            auto _resize(size_t newsize)                          -> void;

            auto _init() -> bool override;

            // Adapt mapping on transform
            virtual auto _onChanged(const sf::Transform& old) -> void override;

        public:
            size_t reserveAhead;  // How many vertices to allocate additionaly to required size

        private:
            TextureResource::Handle _tex;
            math::Vec2f _texoffset;
            math::Vec2f _texscale;
            MappingMethod _mapping;
    };
}

#endif
