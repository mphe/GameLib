#ifndef GAMELIB_COMPONENT_HPP
#define GAMELIB_COMPONENT_HPP

#include "gamelib/core/Identifiable.hpp"
#include "gamelib/utils/SlotMap.hpp"
#include "gamelib/core/res/JsonSerializer.hpp"

namespace gamelib
{
    class Entity;

    class Component : public Identifiable, public JsonSerializer
    {
        friend class Entity;

        typedef SlotKeyShort Handle;

        public:
            Component();
            Component(const std::string& name);
            virtual ~Component() {};

            auto getName() const         -> const std::string&;
            auto getEntity() const       -> Entity*;
            auto getEntityHandle() const -> Handle;

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

        protected:
            // Those are called by Entity
            virtual auto _init()    -> bool;
            virtual auto _quit()    -> void;
            virtual auto _refresh() -> void;

        private:
            Handle _ent;   // Set by Entity
            std::string _name;

            // Only used when the entity was created outside an EntityManager
            Entity* _entptr; // Set by Entity
    };
}

#endif
