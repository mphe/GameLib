#ifndef GAMELIB_ANIMATION_COMPONENT_HPP
#define GAMELIB_ANIMATION_COMPONENT_HPP

#include "gamelib/components/UpdateComponent.hpp"
#include "gamelib/core/sprite/SpriteData.hpp"

// Used in conjunction with SpriteComponent.
// Should usually not instatiated on its own but by other components.
// Dirty hack to get sprites running for now.
// TODO: find a better approach

namespace gamelib
{
    class SpriteComponent;

    class AnimationComponent : public UpdateComponent
    {
        // HACK: allows SpriteComponent to call _init() and _quit() to manually
        //       manage this component.
        // NOTE: this breaks as soon as the component needs to know its entity
        //       because it will never have one.
        // TODO: fix this by implementing some nicer workarounds
        friend class SpriteComponent;

        public:
            constexpr static const char* name = "AnimationComponent";

        public:
            AnimationComponent();
            AnimationComponent(SpriteComponent* sprite);

            auto update(float elapsed) -> void;

        public:
            SpriteComponent* sprite;
            AnimationData ani;
    };
}

#endif
