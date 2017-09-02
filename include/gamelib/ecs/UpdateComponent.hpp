#ifndef GAMELIB_UPDATE_COMPONENT_HPP
#define GAMELIB_UPDATE_COMPONENT_HPP

#include "gamelib/Identifier.hpp"
#include "gamelib/update/Updatable.hpp"
#include "Component.hpp"
#include "gamelib/utils/SlotMap.hpp"

namespace gamelib
{
    class UpdateComponent : public Identifier<0xd0936e0f, Component>,
                            public Updatable
    {
        public:
            typedef SlotKeyShort Handle;

        public:
            UpdateComponent(int interval = 0);
            virtual ~UpdateComponent() {}

        protected:
            virtual auto _init() -> bool;
            virtual auto _quit() -> void;

        private:
            Handle _handle;
    };
}

#endif
