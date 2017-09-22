#ifndef GAMELIB_SPRITE_COMPONENT_HPP
#define GAMELIB_SPRITE_COMPONENT_HPP

#include "gamelib/core/ecs/UpdateComponent.hpp"
#include "gamelib/core/sprite/SpriteData.hpp"
#include "gamelib/core/res/TextureResource.hpp"
#include "math/geometry/AABB.hpp"

// TODO: maybe automatically create PolygonShape

namespace gamelib
{
    class PolygonShape;

    class SpriteComponent : public UpdateComponent
    {
        public:
            constexpr static const char* name = "SpriteComponent";

        public:
            SpriteComponent();
            virtual ~SpriteComponent() {};

            virtual auto update(float elapsed) -> void;

            auto change(const std::string& fname) -> void;
            auto getSpriteName() const            -> const std::string&;

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

        protected:
            auto _refresh() -> void;
            auto _updateShape() -> void;
            auto _initShape() -> void;

        public:
            AnimationData ani; // TODO: make this private and write wrappers for index and speed

        private:
            PolygonShape* _shape;
            TextureResource::Handle _tex;
            std::string _spritename;
    };
}

#endif
