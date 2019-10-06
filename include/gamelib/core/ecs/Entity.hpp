#ifndef GAMELIB_ENTITY_HPP
#define GAMELIB_ENTITY_HPP

#include <string>
#include <vector>
#include <memory>
#include "gamelib/utils/Identifier.hpp"
#include "gamelib/core/geometry/GroupTransform.hpp"
#include "flags.hpp"
#include "ecsmeta.hpp"
#include "json/json.h"

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
    // Deprecated
    typedef EntityReference EntityHandle;

    auto getEntity(EntityHandle handle) -> EntityHandle;


    class Entity : public LifetimeTracker<Entity>
    {
        private:
            struct ComponentData
            {
                unsigned int id;
                ComponentPtr ptr;
            };

        public:
            typedef EntityHandle Handle;    // for backwards compatibility
            typedef std::vector<ComponentData> ComponentList;

        public:
            Entity();
            Entity(const std::string& name);
            ~Entity();

            // Disable move/copy
            Entity(const Entity&) = delete;
            Entity(Entity&& ent) = delete;
            auto operator=(const Entity&) -> Entity& = delete;
            auto operator=(Entity&& rhs) -> Entity& = delete;

            // auto clone()   -> Entity; // TODO Explicit copy might be better here than copy constructor
            auto destroy() -> void;

            auto getName() const      -> const std::string&;

            auto getTransform() const -> const GroupTransform&;
            auto getTransform()       -> GroupTransform&;

            auto add(ComponentPtr comp)              -> Component*;
            auto remove(Component* comp)             -> void;
            auto hasComponent(Component* comp) const -> bool;
            auto find(ID type) const                 -> Component*;
            auto find(const std::string& name) const -> Component*;
            auto size() const                        -> size_t;
            auto clearComponents()                   -> void;

            auto begin() const -> ComponentList::const_iterator;
            auto end() const   -> ComponentList::const_iterator;

            auto addChild(EntityPtr ent)              -> EntityReference;
            auto popChild(EntityReference ent)        -> EntityPtr;
            auto popChild(size_t index)               -> EntityPtr;
            auto reparent(EntityReference ent)        -> EntityReference;
            auto getChildren() const                  -> const std::vector<EntityPtr>&;
            auto getParent() const                    -> EntityReference;
            auto isChildOf(EntityReference ent) const -> bool;

            // Iterate over the hierachy.
            // Returns the entity breaked at, otherwise null.
            // Return true to break loop, otherwise false.
            // Signature: (EntityReference) -> bool
            template <typename F>
            auto iterSubtree(F f) const -> EntityReference;


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
            friend bool extendFromJson(const Json::Value&, Entity&, bool);

        private:
            auto _quit() -> void;
            auto _refresh(RefreshType type, Component* comp) -> void;

        public:
            unsigned int flags;

        private:
            std::string _name;
            GroupTransform _transform;
            bool _clearing;
            ComponentList _components;
            EntityReference _parent;
            std::vector<EntityPtr> _children;
    };
}

#include "Component.hpp"

namespace gamelib
{

    // template <typename T>
    // T* findComponentByName(const std::string& entname)
    // {
    //     auto ent = findEntity(entname);
    //     return ent ? ent->findByName<T>() : nullptr;
    // }

    // template <typename T>
    // T* findComponentByType(const std::string& entname)
    // {
    //     auto ent = findEntity(entname);
    //     return ent ? ent->findByType<T>() : nullptr;
    // }



    template <typename T, typename... Args>
    auto Entity::add(Args&&... args) -> T*
    {
        auto comp = add(ComponentPtr(new T(std::forward<Args>(args)...)));
        return static_cast<T*>(comp);
    }


    template <typename T>
    auto Entity::findByType() const -> T*
    {
        static_assert(has_identifier<T>::value, "Only works for types derived from gamelib::Identifier");
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
        static_assert(has_identifier<T>::value, "Only works for types derived from gamelib::Identifier");
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

    template <typename F>
    auto Entity::iterSubtree(F f) const -> EntityReference
    {
        for (auto& i : _children)
        {
            if (f(i.get()))
                return i.get();

            auto tmp = i->iterSubtree(f);
            if (tmp)
                return tmp;
        }
        return nullptr;
    }
}


#endif
