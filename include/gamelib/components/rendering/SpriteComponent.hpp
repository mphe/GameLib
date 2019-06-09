#ifndef GAMELIB_SPRITE_COMPONENT_HPP
#define GAMELIB_SPRITE_COMPONENT_HPP

#include "gamelib/components/RenderComponent.hpp"
#include "gamelib/components/update/AnimationComponent.hpp"
#include "gamelib/core/res/SpriteResource.hpp"

// Has to be a render component because it needs custom imgui support

/*
 * Config file structure:
 * {
 *     <RenderComponent entries>,
 *
 *     "sprite": "filename"
 * }
 */

namespace gamelib
{
    class AnimationComponent;

    class SpriteComponent : public RenderComponent
    {
        friend class AnimationComponent;

        public:
            ASSIGN_NAMETAG("SpriteComponent");

        public:
            SpriteComponent();

            auto change(const std::string& fname)      -> void;
            auto change(SpriteResource::Handle sprite) -> void;
            auto setIndex(int index)                   -> void;
            auto setSpeed(float speed)                 -> void;

            auto getSprite() const     -> SpriteResource::Handle;
            auto getSpriteName() const -> const std::string&;
            auto getAnimation() const  -> const AnimationComponent&;
            auto getTexture() const    -> TextureResource::Handle;

        protected:
            auto _init()    -> bool final override;
            auto _quit()    -> void final override;
            auto _initShape() -> void;
            auto _updateUV()  -> void;

        private:
            SpriteResource::Handle _sprite;
            AnimationComponent _ani;
    };
}

#endif
