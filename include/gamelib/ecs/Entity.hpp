#ifndef GAMELIB_ENTITY_HPP
#define GAMELIB_ENTITY_HPP

#include <string>
#include <vector>
#include <memory>
#include "gamelib/utils/SlotMap.hpp"
#include "gamelib/Identifier.hpp"
#include "gamelib/geometry/GroupTransform.hpp"
#include "Component.hpp"

// TODO: Cache the current System or take it as parameter, so that entities
//       can be created in a specific System.
//       Also pass the System pointer to components.

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

            auto getHandle() const    -> Handle;
            auto getName() const      -> const std::string&;

            auto getTransform() const -> const GroupTransform&;
            auto getTransform()       -> GroupTransform&;

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

            template <typename T, typename... Args>
            auto add(Args&&... args) -> T*
            {
                auto comp = add(ComponentPtr(new T(std::forward<Args>(args)...)));
                return static_cast<T*>(comp);
            }

            template <typename T>
            auto find() const -> T*
            {
                static_assert(isIdentifiable<T>::value, "Only works for types derived from gamelib::Identifier");
                return static_cast<T*>(find(T::id));
            }

            // Calls a callback for each found component.
            // Signature: bool(Component*)
            // If the lambda returns true, the loop breaks. To continue return false.
            template <typename F>
            auto findAll(ID type, F callback) const -> void
            {
                for (auto it = _components.begin(), end = _components.end(); it != end; ++it)
                    if ((*it)->getID() == type)
                        if (callback(it->get()))
                            break;
            }

            // Calls a callback for each found T component.
            // Signature: bool(T*)
            // If the lambda returns true, the loop breaks. To continue return false.
            template <typename T, typename F>
            auto findAll(F callback) const -> void
            {
                static_assert(isIdentifiable<T>::value, "Only works for types derived from gamelib::Identifier");
                findAll(T::id, [&](Component* comp) {
                        return callback(static_cast<T*>(comp));
                    });
            }

        private:
            auto _init() -> void;   // Called by EntityManager when entity was added
            auto _quit() -> void;   // Called by EntityManager when entity was removed
            auto _refresh() -> void;

        private:
            EntityManager* _entmgr; // Set by EntityManager
            Handle _handle;         // Set by EntityManager
            std::string _name;
            GroupTransform _transform;
            bool _quitting;
            ComponentList _components;
    };

    auto getEntity(Entity::Handle handle) -> Entity*;
}

#endif
