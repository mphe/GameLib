#ifndef GAMELIB_COMPONENT_HPP
#define GAMELIB_COMPONENT_HPP

#include "gamelib/utils/utils.hpp"
#include "gamelib/utils/Identifiable.hpp"
#include "gamelib/utils/nametag.hpp"
#include "gamelib/utils/LifetimeTracker.hpp"
#include "gamelib/properties/PropertyContainer.hpp"
#include "gamelib/json/JsonSerializer.hpp"
#include "ecsmeta.hpp"


namespace gamelib
{
    class Transformable;

    class Component : public Identifiable, public INameTag, public JsonSerializer, public LifetimeTracker<Component>
    {
        friend class Entity;

        public:
            Component();
            Component(const Component&) = delete;   // Prevent shooting in the foot
            Component(Component&&) = delete;   // Prevent shooting in the foot
            virtual ~Component() {};

            auto init()                -> bool;
            auto quit()                -> void;
            auto isInitialized() const -> bool;

            auto getEntity() const       -> EntityReference;
            auto getProperties() const   -> const PropertyContainer&;

            virtual auto getTransform()       -> Transformable*;
            virtual auto getTransform() const -> const Transformable*;

            virtual auto loadFromJson(const Json::Value& node) -> bool override;
            virtual auto writeToJson(Json::Value& node) const  -> void override;

        protected:
            virtual auto _init() -> bool { return true; };
            virtual auto _quit() -> void {};

            // Called by Entity
            virtual auto _refresh(UNUSED RefreshType type, UNUSED Component* comp) -> void {};

        protected:
            PropertyContainer _props;

        private:
            EntityReference _entptr; // Set by Entity
            bool _initialized;
    };
}

#endif
