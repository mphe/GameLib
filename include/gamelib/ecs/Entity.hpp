#ifndef GAMELIB_ENTITY_HPP
#define GAMELIB_ENTITY_HPP

#include <string>
#include <vector>
#include <memory>
#include "gamelib/utils/SlotMap.hpp"
#include "gamelib/Identifier.hpp"
#include "Component.hpp"

namespace gamelib
{
    class EntityManager;

    typedef std::unique_ptr<Component> ComponentPtr;

    class Entity
    {
        friend class EntityManager;

        public:
            typedef SlotKeyShort Handle;
            typedef std::vector<ComponentPtr> ComponentList;

        public:
            Entity();
            Entity(const std::string& name);
            // Entity(const Entity& ent);  // TODO or maybe not
            Entity(Entity&& ent) = default;
            ~Entity();

            auto clone()   -> Entity; // TODO Explicit copy might be better here than copy constructor
            auto destroy() -> void;

            auto getHandle() const -> Handle;
            auto getName() const   -> const std::string&;

            auto add(ComponentPtr comp)  -> Component*;
            auto remove(Component* comp) -> void;
            auto find(ID type) const     -> Component*;
            auto size() const            -> size_t;

            auto begin() const -> ComponentList::const_iterator;
            auto begin()       -> ComponentList::iterator;
            auto end() const   -> ComponentList::const_iterator;
            auto end()         -> ComponentList::iterator;

            // auto operator=(const Entity& rhs) -> Entity&;   // TODO or maybe not
            auto operator=(Entity&& rhs) -> Entity& = default;

            template <typename T>
            auto find() const -> T*
            {
                static_assert(isIdentifiable<T>::value, "Only works for types derived from gamelib::Identifier");
                return static_cast<T*>(find(T::id));
            }

        private:
            auto _quit() -> void;   // Called by EntityManager when entity was removed
            auto _refresh() -> void;

        private:
            EntityManager* _entmgr; // Set by EntityManager
            Handle _handle;         // Set by EntityManager
            std::string _name;
            ComponentList _components;
    };
}

#endif
