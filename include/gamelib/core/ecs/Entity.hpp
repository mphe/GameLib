#ifndef GAMELIB_ENTITY_HPP
#define GAMELIB_ENTITY_HPP

#include <string>
#include <vector>
#include "gamelib/utils/Identifier.hpp"
#include "gamelib/core/geometry/GroupTransform.hpp"
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
    class Entity : public LifetimeTracker<Entity>
    {
        private:
            struct ComponentData
            {
                unsigned int id;
                ComponentPtr ptr;
            };

        public:
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

            auto add(ComponentPtr comp)                 -> BaseCompRef;
            auto remove(BaseCompRef comp)             -> void;
            auto hasComponent(BaseCompRef comp) const -> bool;
            auto find(ID type) const                    -> BaseCompRef;
            auto find(const std::string& name) const    -> BaseCompRef;
            auto size() const                           -> size_t;
            auto clearComponents()                      -> void;

            auto begin() const -> ComponentList::const_iterator;
            auto end() const   -> ComponentList::const_iterator;

            auto addChild(EntityPtr ent, bool keepTransform = true)       -> EntityReference;
            auto popChild(size_t index, bool keepTransform = true)        -> EntityPtr;
            auto popChild(EntityReference ent, bool keepTransform = true) -> EntityPtr;
            auto reparent(EntityReference ent, bool keepTransform = true) -> EntityReference;
            auto getChildren() const                                      -> const std::vector<EntityPtr>&;
            auto getParent() const                                        -> EntityReference;
            auto isChildOf(EntityReference ent) const                     -> bool;

            // Iterate over the hierachy.
            // Returns the entity breaked at, otherwise null.
            // Return true to break loop, otherwise false.
            // Signature: (EntityReference) -> bool
            template <typename F>
            auto iterSubtree(F f, bool skipself = false) const -> EntityReference;


            template <typename T, typename... Args>
            auto add(Args&&... args) -> ComponentReference<T>;

            template <typename T>
            auto findByType() const -> ComponentReference<T>;

            template <typename T>
            auto findByName() const -> ComponentReference<T>;

            // Calls a callback for each found component.
            // Signature: bool(BaseCompRef)
            // If the lambda returns true, the loop breaks.
            // Returns the component where the loop breaked, otherwise null.
            template <typename F>
            auto findAll(ID type, F callback) const -> BaseCompRef;

            template <typename F>
            auto findAll(const std::string& name, F callback) const -> BaseCompRef;

            // Calls a callback for each found T component.
            // Signature: bool(ComponentReference<T>)
            // If the lambda returns true, the loop breaks.
            // Returns the component where the loop breaked, otherwise null.
            template <typename T, typename F>
            auto findAllByType(F callback) const -> ComponentReference<T>;

            template <typename T, typename F>
            auto findAllByName(F callback) const -> ComponentReference<T>;

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
    template <typename T, typename... Args>
    auto Entity::add(Args&&... args) -> ComponentReference<T>
    {
        return add(ComponentPtr(new T(std::forward<Args>(args)...))).as<T>();
    }


    template <typename T>
    auto Entity::findByType() const -> ComponentReference<T>
    {
        static_assert(has_identifier<T>::value, "Only works for types derived from gamelib::Identifier");
        return find(T::id).template as<T>();
    }

    template <typename T>
    auto Entity::findByName() const -> ComponentReference<T>
    {
        static_assert(has_nametag<T>(), "Only works for types with a nametag");
        return find(T::name()).template as<T>();
    }

    template <typename T, typename F>
    auto Entity::findAllByType(F callback) const -> ComponentReference<T>
    {
        static_assert(has_identifier<T>::value, "Only works for types derived from gamelib::Identifier");
        return findAll(T::id, [&](BaseCompRef comp) {
                return callback(comp.as<T>());
            }).template as<T>();
    }

    template <typename T, typename F>
    auto Entity::findAllByName(F callback) const -> ComponentReference<T>
    {
        static_assert(has_nametag<T>(), "Only works for types with a nametag");
        return findAll(T::name(), [&](BaseCompRef comp) {
                return callback(comp.as<T>());
            }).template as<T>();
    }


    template <typename F>
    auto Entity::findAll(ID type, F callback) const -> BaseCompRef
    {
        for (const auto& i : _components)
            if (i.ptr->getID() == type && callback(i.ptr.get()))
                return i.ptr.get();
        return nullptr;
    }

    template <typename F>
    auto Entity::findAll(const std::string& name, F callback) const -> BaseCompRef
    {
        for (const auto& i : _components)
            if (i.ptr->getName() == name && callback(i.ptr.get()))
                return i.ptr.get();
        return nullptr;
    }

    template <typename F>
    auto Entity::iterSubtree(F f, bool skipself) const -> EntityReference
    {
        if (!skipself)
            if (f(const_cast<Entity*>(this)))
                return const_cast<Entity*>(this);

        for (auto& i : _children)
        {
            auto tmp = i->iterSubtree(f, false);
            if (tmp)
                return tmp;
        }
        return nullptr;
    }
}


#endif
