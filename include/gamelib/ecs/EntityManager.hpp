#ifndef GAMELIB_ENTITYMANAGER_HPP
#define GAMELIB_ENTITYMANAGER_HPP

#include <memory>
#include "gamelib/utils/SlotMap.hpp"
#include "gamelib/ecs/Entity.hpp"

namespace gamelib
{
    class EntityManager
    {
        public:
            typedef SlotMapShort<Entity>::Handle Handle;

        public:
            auto add(const std::string& name = "unknown") -> Entity::Handle;
            // auto add(Entity&& ent)                        -> Entity&;
            auto destroy(Handle handle)                   -> void;
            auto get(Handle handle) const                 -> const Entity*;
            auto get(Handle handle)                       -> Entity*;

            auto clear() -> void;

            // Returns the first entity with the given name
            auto find(const std::string& name) const -> const Entity*;
            auto find(const std::string& name)       -> Entity*;

        private:
            gamelib::SlotMapShort<Entity> _entities;
    };
}

#endif
