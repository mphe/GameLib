#ifndef GAMELIB_ENTITY_HPP
#define GAMELIB_ENTITY_HPP

#include <string>
#include <vector>
#include <memory>
#include "gamelib/utils/SlotMap.hpp"
#include "gamelib/core/Identifier.hpp"
#include "gamelib/core/geometry/GroupTransform.hpp"
#include "Component.hpp"
#include "flags.hpp"

/*
 * Config file structure:
 * {
 *     "name": <str>,
 *     "transform": {
 *         "pos": [ <float>, <float> ],
 *         "scale": [ <float>, <float> ],
 *         "angle": <float>,
 *     },
 *     "components": [
 *         {
 *             "name": <str>
 *             ...
 *         },
 *         ...
 *     ]
 * }
 */

namespace gamelib
{
    class EntityManager;

    typedef std::unique_ptr<Component> ComponentPtr;

    class Entity
    {
        friend class EntityManager;

        private:
            struct ComponentData
            {
                unsigned int id;
                ComponentPtr ptr;
            };

        public:
            typedef SlotKeyShort Handle;
            typedef std::vector<ComponentData> ComponentList;

        public:
            Entity();
            Entity(const std::string& name);
            Entity(Entity&& ent);
            ~Entity();

            auto operator=(Entity&& rhs) -> Entity&;

            // auto clone()   -> Entity; // TODO Explicit copy might be better here than copy constructor
            auto destroy() -> void;

            auto getHandle() const    -> Handle;
            auto getName() const      -> const std::string&;

            auto getTransform() const -> const GroupTransform&;
            auto getTransform()       -> GroupTransform&;

            auto add(ComponentPtr comp)              -> Component*;
            auto remove(Component* comp)             -> void;
            auto find(ID type) const                 -> Component*;
            auto find(const std::string& name) const -> Component*;
            auto size() const                        -> size_t;
            auto clear()                             -> void;

            auto begin() const -> ComponentList::const_iterator;
            auto end() const   -> ComponentList::const_iterator;

            // Iterate over each component
            // Signature: bool(Component*)
            // If the lambda returns false, the loop breaks. To continue return true.
            template <typename F>
            auto foreach(F f) const -> void;

            template <typename T, typename... Args>
            auto add(Args&&... args) -> T*;

            template <typename T>
            auto findByType() const -> T*;

            template <typename T>
            auto findByName() const -> T*;

            // Calls a callback for each found component.
            // Signature: bool(Component*)
            // If the lambda returns true, the loop breaks. To continue return false.
            template <typename F>
            auto findAll(ID type, F callback) const -> void;

            // Calls a callback for each found component.
            // Signature: bool(Component*)
            // If the lambda returns true, the loop breaks. To continue return false.
            template <typename F>
            auto findAll(const std::string& name, F callback) const -> void;

            // Calls a callback for each found T component.
            // Signature: bool(T*)
            // If the lambda returns true, the loop breaks. To continue return false.
            template <typename T, typename F>
            auto findAllByType(F callback) const -> void;

            // Calls a callback for each found T component.
            // Signature: bool(T*)
            // If the lambda returns true, the loop breaks. To continue return false.
            template <typename T, typename F>
            auto findAllByName(F callback) const -> void;

        public:
            unsigned int flags;

        private:
            auto _init() -> void;   // Called by EntityManager when entity was added
            auto _quit() -> void;   // Called by EntityManager when entity was removed
            auto _refresh(RefreshType type, Component* comp) -> void;

        private:
            EntityManager* _entmgr; // Set by EntityManager
            Handle _handle;         // Set by EntityManager
            std::string _name;
            GroupTransform _transform;
            bool _quitting;
            ComponentList _components;

        public:
            template <typename F>
            friend void writeToJson(Json::Value&, const Entity&, F);
            friend bool extendFromJson(const Json::Value&, Entity&, bool);
    };


    auto getEntity(Entity::Handle handle) -> Entity*;
    auto findEntity(const std::string& name) -> Entity*;



    template <typename F>
    auto Entity::foreach(F f) const -> void
    {
        for (const auto& i : _components)
            if (!f(i.ptr.get()))
                return;
    }

    template <typename T, typename... Args>
    auto Entity::add(Args&&... args) -> T*
    {
        auto comp = add(ComponentPtr(new T(std::forward<Args>(args)...)));
        return static_cast<T*>(comp);
    }


    template <typename T>
    auto Entity::findByType() const -> T*
    {
        static_assert(isIdentifiable<T>::value, "Only works for types derived from gamelib::Identifier");
        return static_cast<T*>(find(T::id));
    }

    template <typename T>
    auto Entity::findByName() const -> T*
    {
        static_assert(has_nametag<T>(), "Only works for types with a nametag");
        return static_cast<T*>(find(T::name()));
    }

    template <typename T, typename F>
    auto Entity::findAllByType(F callback) const -> void
    {
        static_assert(isIdentifiable<T>::value, "Only works for types derived from gamelib::Identifier");
        findAll(T::id, [&](Component* comp) {
                return callback(static_cast<T*>(comp));
            });
    }

    template <typename T, typename F>
    auto Entity::findAllByName(F callback) const -> void
    {
        static_assert(has_nametag<T>(), "Only works for types with a nametag");
        findAll(T::name(), [&](Component* comp) {
                return callback(static_cast<T*>(comp));
            });
    }


    template <typename F>
    auto Entity::findAll(ID type, F callback) const -> void
    {
        for (const auto& i : _components)
            if (i.ptr->getID() == type)
                if (callback(i.ptr.get()))
                    break;
    }

    template <typename F>
    auto Entity::findAll(const std::string& name, F callback) const -> void
    {
        for (const auto& i : _components)
            if (i.ptr->getName() == name)
                if (callback(i.ptr.get()))
                    break;
    }
}


#endif
