#include "gamelib/components/update/AnimationComponent.hpp"
#include "gamelib/components/rendering/SpriteComponent.hpp"

namespace gamelib
{
    AnimationComponent::AnimationComponent() :
        AnimationComponent(nullptr)
    { }

    AnimationComponent::AnimationComponent(SpriteComponent* sprite_) :
        UpdateComponent(1, UpdateHookType::PreFrame),
        sprite(sprite_)
    { }

    void AnimationComponent::update(float elapsed)
    {
        int old = ani.offset;
        ani.update(elapsed);

        if (old != (int)ani.offset)
        {
            assert(sprite && "No sprite assigned");
            sprite->_updateUV();
        }
    }
}
