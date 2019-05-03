#ifndef GAMELIB_COMPONENT_HPP
#define GAMELIB_COMPONENT_HPP

#include "gamelib/core/Identifiable.hpp"
#include "gamelib/utils/SlotMap.hpp"
#include "gamelib/properties/PropertyContainer.hpp"
#include "gamelib/json/JsonSerializer.hpp"

namespace gamelib
{
    class Entity;
    class Transformable;

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
            auto getProperties() const   -> const PropertyContainer&;
            virtual auto getTransform()       -> Transformable*;
            virtual auto getTransform() const -> const Transformable*;

            virtual auto loadFromJson(const Json::Value& node) -> bool override;
            virtual auto writeToJson(Json::Value& node) const  -> void override;

        protected:
            // Those are called by Entity
            virtual auto _init()    -> bool;
            virtual auto _quit()    -> void;
            virtual auto _refresh() -> void;

        protected:
            PropertyContainer _props;
            std::string _name;

        private:
            Handle _ent;   // Set by Entity

            // Only used when the entity was created outside an EntityManager
            Entity* _entptr; // Set by Entity
    };
}

#endif
