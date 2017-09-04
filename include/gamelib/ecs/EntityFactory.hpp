#ifndef GAMELIB_ENTITYFACTORY_HPP
#define GAMELIB_ENTITYFACTORY_HPP

#include "gamelib/utils/Factory.hpp"
#include "gamelib/res/predefs.hpp"
#include "Component.hpp"
#include "Entity.hpp"
#include "json/json.h"

// TODO: Maybe don't store JsonResource::Handle but Json::Value
//       (by making a copy).

namespace gamelib
{
    class EntityFactory //: public JsonSerializer
    {
        typedef Factory<Component, std::string> ComponentFactory;

        public:
            EntityFactory();

            auto createFromJson(const Json::Value& node)              -> Entity::Handle;
            auto createFromJson(const Json::Value& node, Entity* ent) -> void;
            auto create(const std::string& name)                  -> Entity::Handle;
            auto create(const std::string& name, Entity* ent)     -> void;
            auto createComponent(const std::string& name)         -> ComponentPtr;
            auto createComponentFromJson(const Json::Value& node) -> ComponentPtr;

            auto add(JsonResource::Handle res)                          -> void;
            auto add(const std::string& name, JsonResource::Handle res) -> void;
            auto addComponent(const std::string& name, ComponentFactory::CreatorFunction callback) -> void;

            auto removeEntity(const std::string& name)    -> void;
            auto removeComponent(const std::string& name) -> void;
            auto clear() -> void;

            // auto loadFromJson(const Json::Value& node) -> bool;
            // auto writeToJson(Json::Value& node)        -> void;

            template <typename T>
            void addComponent(const std::string& name)
            {
                _compfactory.add<T>(name);
            }

        private:
            std::unordered_map<std::string, JsonResource::Handle> _entdata;
            ComponentFactory _compfactory;
    };
}

#endif
