#ifndef GAMELIB_ENTITY_HPP
#define GAMELIB_ENTITY_HPP

#include <string>
#include <vector>
#include <memory>
#include "gamelib/utils/SlotMap.hpp"
#include "gamelib/core/Identifier.hpp"
#include "gamelib/core/geometry/GroupTransform.hpp"
#include "gamelib/utils/json.hpp"
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

    class Entity : public JsonSerializer
    {
        friend class EntityManager;

        public:
            typedef SlotKeyShort Handle;
            typedef std::vector<ComponentPtr> ComponentList;

        public:
            Entity();
            Entity(const std::string& name);
            Entity(Entity&& ent);
            ~Entity();

            auto operator=(Entity&& rhs) -> Entity&;

            // auto clone()   -> Entity; // TODO Explicit copy might be better here than copy constructor
            auto destroy() -> void;

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

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
            auto begin()       -> ComponentList::iterator;
            auto end() const   -> ComponentList::const_iterator;
            auto end()         -> ComponentList::iterator;

            template <typename T, typename... Args>
            auto add(Args&&... args) -> T*
            {
                auto comp = add(ComponentPtr(new T(std::forward<Args>(args)...)));
                return static_cast<T*>(comp);
            }

            template <typename T>
            auto findByType() const -> T*
            {
                static_assert(isIdentifiable<T>::value, "Only works for types derived from gamelib::Identifier");
                return static_cast<T*>(find(T::id));
            }

            template <typename T>
            auto findByName() const -> T*
            {
                return static_cast<T*>(find(T::name));
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
            auto findAllByType(F callback) const -> void
            {
                static_assert(isIdentifiable<T>::value, "Only works for types derived from gamelib::Identifier");
                findAll(T::id, [&](Component* comp) {
                        return callback(static_cast<T*>(comp));
                    });
            }

            // Calls a callback for each component about to be serialized.
            // The callback handles the serialization (or not).
            // Signature: bool(Json::Value&, Component&)
            template <typename F>
            auto writeToJson(Json::Value& node, F callback) const -> void
            {
                node["name"] = _name;
                node["flags"] = flags;
                gamelib::writeToJson(node["transform"], _transform);

                if (flags & entity_exportcomponents)
                {
                    auto& comps = node["components"];
                    comps = Json::Value(Json::arrayValue);

                    for (auto& i : _components)
                        if (i)
                        {
                            Json::Value comp;
                            callback(comp, *i);
                            if (!comp.isNull())
                                comps.append(comp);
                        }
                }
            }

        public:
            unsigned int flags;

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
