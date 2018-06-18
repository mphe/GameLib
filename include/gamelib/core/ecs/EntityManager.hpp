#ifndef GAMELIB_ENTITYMANAGER_HPP
#define GAMELIB_ENTITYMANAGER_HPP

#include <memory>
#include "gamelib/utils/SlotMap.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/Subsystem.hpp"

namespace gamelib
{
    class EntityManager : public Subsystem<EntityManager>
    {
        public:
            typedef SlotMapShort<Entity>::Handle Handle;

            constexpr static const char* name = "EntityManager";

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

            auto begin() const -> gamelib::SlotMapShort<Entity>::const_iterator;
            auto begin()       -> gamelib::SlotMapShort<Entity>::iterator;
            auto end() const   -> gamelib::SlotMapShort<Entity>::const_iterator;
            auto end()         -> gamelib::SlotMapShort<Entity>::iterator;

        private:
            gamelib::SlotMapShort<Entity> _entities;
    };
}

#endif
