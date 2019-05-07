#include "gamelib/components/update/ActorComponent.hpp"
#include "gamelib/components/update/QPhysics.hpp"
#include "gamelib/components/rendering/SpriteComponent.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/input/InputSystem.hpp"

namespace gamelib
{
    constexpr const char* ActorComponent::state_strings[];

    ActorComponent::ActorComponent() :
        UpdateComponent(name),
        turnspeed(15),
        followMouse(false),
        freeze(false),
        _sprite(nullptr),
        _phys(nullptr),
        _current(Stand)
    {
        _props.registerProperty("turnspeed", turnspeed);
        _props.registerProperty("followMouse", followMouse);
        _props.registerProperty("freeze", freeze);
        _props.registerProperty("stand", _states[Stand]);
        _props.registerProperty("walk", _states[Walk]);
        _props.registerProperty("fall", _states[Fall]);
        _props.registerProperty("state", _current, PROP_METHOD(_current, changeState), this, 0, NumStates, state_strings);
    }

    void ActorComponent::changeState(State state)
    {
        if (state != _current && _sprite)
        {
            if (_states[state])
                _sprite->change(_states[state]);
            else
                LOG_DEBUG_WARN("No sprite specified for state: ", state_strings[state]);
        }
        _current = state;
    }

    void ActorComponent::update(float elapsed)
    {
        if (freeze)
            return;

        math::Vec2f eyedir;

        if (_phys)
        {
            eyedir = _phys->vel;

            if (_phys->getState() == QPhysics::Air)
                changeState(Fall);
            else if (eyedir.x == 0)
                changeState(Stand);
            else
                changeState(Walk);
        }

        if (followMouse)
        {
            auto input = getSubsystem<InputSystem>();
            if (input)
                eyedir = input->getMouse().world - getEntity()->getTransform().getPosition();
        }

        if (_sprite)
        {
            auto scale = _sprite->getScale();

            if (eyedir.x < 0)
                scale.x = std::fmax(-1, scale.x - turnspeed * elapsed);
            else if (eyedir.x > 0)
                scale.x = std::fmin(1, scale.x + turnspeed * elapsed);
            else
                scale.x = scale.x < 0 ? -1 : 1;

            _sprite->setScale(scale);
        }
    }

    void ActorComponent::_refresh(RefreshType type, Component* comp)
    {
        _sprite = getEntity()->findByName<SpriteComponent>();
        _phys = getEntity()->findByName<QPhysics>();
    }
}
