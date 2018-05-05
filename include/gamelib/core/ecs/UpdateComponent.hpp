#ifndef GAMELIB_UPDATE_COMPONENT_HPP
#define GAMELIB_UPDATE_COMPONENT_HPP

#include "gamelib/core/update/UpdateSystem.hpp"
#include "Component.hpp"

namespace gamelib
{
    class UpdateComponent : public Identifier<0xd0936e0f, Component>,
                            public Updatable
    {
        public:
            typedef SlotKeyShort Handle;

        public:
            UpdateComponent(const std::string& name, int interval = 1, UpdateSystem::HookType hook = UpdateSystem::Frame);
            virtual ~UpdateComponent() {}

            virtual auto loadFromJson(const Json::Value& node) -> bool;
            virtual auto writeToJson(Json::Value& node)        -> void;

        protected:
            virtual auto _init() -> bool;
            virtual auto _quit() -> void;

        private:
            Handle _handle;
            UpdateSystem::HookType _hook;
    };
}

#endif
