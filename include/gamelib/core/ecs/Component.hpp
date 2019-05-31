#ifndef GAMELIB_COMPONENT_HPP
#define GAMELIB_COMPONENT_HPP

#include "gamelib/utils/Identifiable.hpp"
#include "gamelib/utils/SlotMap.hpp"
#include "gamelib/utils/nametag.hpp"
#include "gamelib/properties/PropertyContainer.hpp"
#include "gamelib/json/JsonSerializer.hpp"

namespace gamelib
{
    class Entity;
    class Transformable;

    enum RefreshType
    {
        ComponentAdded,     // A component was added to the entity
        ComponentRemoved,   // A component was removed from the entity
        PostLoad,           // All configs were loaded (in entity creation)
    };

    class Component : public Identifiable, public INameTag, public JsonSerializer
    {
        friend class Entity;

        typedef SlotKeyShort Handle;

        public:
            Component();
            Component(const Component&) = delete;   // Prevent shooting in the foot
            virtual ~Component() {};

            auto init()                -> bool;
            auto quit()                -> void;
            auto isInitialized() const -> bool;

            auto getEntity() const       -> Entity*;
            auto getEntityHandle() const -> Handle;
            auto getProperties() const   -> const PropertyContainer&;

            virtual auto getTransform()       -> Transformable*;
            virtual auto getTransform() const -> const Transformable*;

            virtual auto loadFromJson(const Json::Value& node) -> bool override;
            virtual auto writeToJson(Json::Value& node) const  -> void override;

        protected:
            virtual auto _init() -> bool { return true; };
            virtual auto _quit() -> void {};

            // Called by Entity
            virtual auto _refresh(RefreshType type, Component* comp) -> void {};

        protected:
            PropertyContainer _props;

        private:
            Handle _ent;   // Set by Entity
            // Only used when the entity was created outside an EntityManager
            Entity* _entptr; // Set by Entity
            bool _initialized;
    };
}

#endif
