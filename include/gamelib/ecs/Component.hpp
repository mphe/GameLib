#ifndef GAMELIB_COMPONENT_HPP
#define GAMELIB_COMPONENT_HPP

#include "gamelib/Identifiable.hpp"

// TODO: Get the System from Entity

namespace gamelib
{
    class Entity;

    class Component : public Identifiable
    {
        friend class Entity;

        public:
            Component();
            virtual ~Component() {};

            auto getEntity() const -> Entity*;

        protected:
            // Those are called by Entity
            virtual auto _init()    -> bool;
            virtual auto _quit()    -> void;
            virtual auto _refresh() -> void;

        private:
            Entity* _ent;   // Set by Entity
    };
}

#endif
