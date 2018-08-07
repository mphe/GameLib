#ifndef GAMELIB_COMPONENT_HPP
#define GAMELIB_COMPONENT_HPP

#include "gamelib/core/res/JsonSerializer.hpp"
#include "gamelib/core/Identifiable.hpp"
#include "gamelib/utils/SlotMap.hpp"
#include "gamelib/utils/Property.hpp"
#include "gamelib/utils/Factory.hpp"


#define REGISTER_COMPONENT(name) struct _##name_Registrator { _##name_Registrator() { gamelib::Component::registerComponent<name>(#name); } } _##name_registrator;


namespace gamelib
{
    class Entity;

    class Component : public Identifiable, public JsonSerializer
    {
        friend class Entity;

        typedef SlotKeyShort Handle;
        typedef Factory<Component, std::string> ComponentFactory;

        public:
            Component();
            Component(const std::string& name);
            virtual ~Component() {};

            auto getName() const         -> const std::string&;
            auto getEntity() const       -> Entity*;
            auto getEntityHandle() const -> Handle;
            auto getProperties() const   -> const PropertyContainer&;

            virtual auto loadFromJson(const Json::Value& node) -> bool;
            virtual auto writeToJson(Json::Value& node)        -> void;

            template <typename T>
            static auto registerComponent(const std::string& name) -> void;
            static auto registerComponent(const std::string& name, ComponentFactory::CreatorFunction callback) -> void;
            static auto unregisterComponent(const std::string& name) -> void;
            static auto getFactory() -> ComponentFactory&;

        protected:
            // Those are called by Entity
            virtual auto _init()    -> bool;
            virtual auto _quit()    -> void;
            virtual auto _refresh() -> void;

        protected:
            PropertyContainer _props;

        private:
            Handle _ent;   // Set by Entity
            std::string _name;

            // Only used when the entity was created outside an EntityManager
            Entity* _entptr; // Set by Entity

        private:
            static ComponentFactory _compfactory;
    };


    template <typename T>
    auto Component::registerComponent(const std::string& name) -> void
    {
        _compfactory.add<T>(name);
        LOG_DEBUG("Registered component ", name);
    }


    // namespace detail
    // {
    //     struct _general {};
    //     struct _special : public _general {};
    //
    //     template <typename T, typename = decltype(T::name)>
    //     constexpr bool _hasname(_special) { return true; }
    //
    //     template <typename T>
    //     constexpr bool _hasname(_general) { return false; }
    //
    // }
    //
    //
    // template <typename T>
    // constexpr bool hasname() { return detail::_hasname<T>(detail::_special()); }
    //
    //
    // template <typename T, typename C>
    // class AutoregComponent : public C
    // {
    //     public:
    //         AutoregComponent() { __registrator.force = true; };
    //
    //         template <typename... Args>
    //         AutoregComponent(const Args&... args) : C(args...) { __registrator.force = true; };
    //
    //         template <typename... Args>
    //         AutoregComponent(Args&&... args) : C(std::forward<Args>(args)...) { __registrator.force = true; };
    //
    //         virtual ~AutoregComponent() {}
    //
    //     private:
    //         static struct __Registrator
    //         {
    //             __Registrator()
    //             {
    //                 static_assert(hasname<T>(), "Component needs a string name member");
    //                 Component::registerComponent<T>(T::name);
    //             }
    //
    //             // __registrator only gets instantiated if it's accessed
    //             // somewhere. Calling this function in AutoregComponent's
    //             // constructor is enough to make it work.
    //             // void force() {}
    //             bool force;
    //         } __registrator;
    // };
    //
    // template <typename T, typename C>
    // typename AutoregComponent<T, C>::__Registrator AutoregComponent<T, C>::__registrator;
}

#endif
