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
            constexpr static const char* name = "ActorComponent";

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

            virtual auto update(float elapsed) -> void;

        protected:
            virtual auto _refresh() -> void;

        public:
            float turnspeed;
            bool followMouse;
            bool freeze;

        private:
            SpriteResource::Handle _states[NumStates];
            SpriteComponent* _sprite;
            QPhysics* _phys;
            State _current;
    };
}

#endif
