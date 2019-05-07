#ifndef GAMELIB_UPDATE_COMPONENT_HPP
#define GAMELIB_UPDATE_COMPONENT_HPP

#include "gamelib/core/update/UpdateSystem.hpp"
#include "gamelib/core/update/Updatable.hpp"
#include "gamelib/core/ecs/Component.hpp"

namespace gamelib
{
    class UpdateComponent : public Identifier<0xd0936e0f, Component>, public Updatable
    {
        public:
            typedef SlotKeyShort Handle;

        public:
            UpdateComponent(const std::string& name, int interval = 1, UpdateHookType hook = Frame);
            virtual ~UpdateComponent();

            auto setHook(UpdateHookType hook) -> void;
            auto getHook() const              -> UpdateHookType;

        protected:
            virtual auto _init() -> bool override;
            virtual auto _quit() -> void override;

        public:
            int interval;

        private:
            Handle _handle;
            UpdateHookType _hook;
    };
}

#endif
