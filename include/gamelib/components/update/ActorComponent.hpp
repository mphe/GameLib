#ifndef GAMELIB_ACTOR_COMPONENT_HPP
#define GAMELIB_ACTOR_COMPONENT_HPP

#include <vector>
#include "gamelib/components/UpdateComponent.hpp"
#include "gamelib/core/res/SpriteResource.hpp"

namespace gamelib
{
    class SpriteComponent;
    class QPhysics;

    class ActorComponent : public UpdateComponent
    {
        public:
            ASSIGN_NAMETAG("ActorComponent");

            enum State
            {
                Stand,
                Walk,
                Fall,
                NumStates
            };

            constexpr static const char* state_strings[] = { "Stand", "Walk", "Fall" };

        public:
            ActorComponent();
            virtual ~ActorComponent() {};

            auto changeState(State state) -> void;

            virtual auto update(float elapsed) -> void override;

        protected:
            virtual auto _refresh(RefreshType type, Component* comp) -> void override;

        public:
            float turnspeed;
            bool followMouse;
            bool freeze;

        private:
            SpriteResource::Handle _states[NumStates];
            ComponentReference<SpriteComponent> _sprite;
            ComponentReference<QPhysics> _phys;
            State _current;
    };
}

#endif
