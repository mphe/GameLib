#ifndef GAMELIB_ENTITYFACTORY_HPP
#define GAMELIB_ENTITYFACTORY_HPP

#include "gamelib/utils/Factory.hpp"
#include "gamelib/core/res/JsonResource.hpp"
#include "gamelib/core/Subsystem.hpp"
#include "Component.hpp"
#include "Entity.hpp"
#include "json/json.h"

// TODO: Maybe don't store JsonResource::Handle but Json::Value
//       (by making a copy).

namespace gamelib
{
    auto createEntity(const std::string& name)                   -> Entity::Handle;
    auto createEntity(const std::string& name, float x, float y) -> Entity::Handle;


    class EntityFactory : public Subsystem<EntityFactory> //: public JsonSerializer
    {
        typedef Factory<Component, std::string> ComponentFactory;

        public:
            constexpr static const char* name = "EntityFactory";

        public:
            EntityFactory();

            auto createWithDelta(const std::string& name, const Json::Value& node)              -> Entity::Handle;
            auto createWithDelta(const std::string& name, const Json::Value& node, Entity* ent) -> void;
            auto createFromJson(const Json::Value& node)              -> Entity::Handle;
            auto createFromJson(const Json::Value& node, Entity* ent) -> void;
            auto create(const std::string& name)                  -> Entity::Handle;
            auto create(const std::string& name, Entity* ent)     -> void;
            auto createComponent(const std::string& name)         -> ComponentPtr;
            auto createComponentFromJson(const std::string& name, const Json::Value& node) -> ComponentPtr;

            auto add(JsonResource::Handle res)                          -> void;
            auto add(const std::string& name, JsonResource::Handle res) -> void;
            auto addComponent(const std::string& name, ComponentFactory::CreatorFunction callback) -> void;

            auto removeEntity(const std::string& name)    -> void;
            auto removeComponent(const std::string& name) -> void;
            auto clear()       -> void;
            auto size() const  -> size_t;

            template <typename T>
            void addComponent(const std::string& name)
            {
                _compfactory.add<T>(name);
                LOG_DEBUG("Registered component ", name);
            }

            // Signature: bool(const std::string&)
            // When true is returned, the loop breaks.
            template <typename F>
            void iterkeys(F callback) const
            {
                for (auto& i : _entdata)
                    if (callback(i.first))
                        break;
            }

        private:
            std::unordered_map<std::string, JsonResource::Handle> _entdata;
            ComponentFactory _compfactory;
    };
}

#endif
