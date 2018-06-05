#include "gamelib/components/update/AnimationComponent.hpp"
#include "gamelib/components/rendering/SpriteComponent.hpp"

namespace gamelib
{
    AnimationComponent::AnimationComponent() :
        AnimationComponent(nullptr)
    { }

    AnimationComponent::AnimationComponent(SpriteComponent* sprite_) :
        UpdateComponent(name),
        sprite(sprite_)
    {
        PropertyContainer* props = sprite ? &sprite->_props : &_props;
        props->registerProperty("length", ani.length);
        props->registerProperty("speed", ani.speed);
        props->registerProperty("offset", ani.offset);
    }

    void AnimationComponent::update(float elapsed)
    {
        int old = ani.offset;
        ani.update(elapsed);

        if (old != (int)ani.offset)
        {
            assert(sprite && "No sprite assigned");
            sprite->setIndex((int)ani.offset);
        }
    }
}
