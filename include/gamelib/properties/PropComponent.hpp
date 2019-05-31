#ifndef GAMELIB_PROPCOMPONENT_HPP
#define GAMELIB_PROPCOMPONENT_HPP

#include "PropType.hpp"
#include "gamelib/utils/nametag.hpp"
#include "gamelib/utils/Identifier.hpp"

namespace gamelib
{
    class Component;
    class Entity;
    class PropertyContainer;

    class PropComponent : public PropType<0xe73dee69, Component*>
    {
        public:
            bool loadFromJson(const PropertyHandle& prop, Component** ptr, const Json::Value& node) const final override;
            void writeToJson(const PropertyHandle& prop, Component* const* ptr, Json::Value& node) const final override;
            bool drawGui(const PropertyHandle& prop, const std::string& name, Component** ptr) const final override;

            static Entity* getEntity(const PropertyHandle& prop);
    };

    extern PropComponent propComponent;

    template <typename T, typename U>
    using ComponentPropSetter = NicePropSetterCallback<T*, U>;


    template <typename T, typename U>
    void registerProperty(PropertyContainer& props, const std::string& name, T*& prop, U& self, NicePropSetterCallback<T*, U> setter,
            unsigned int filter, int max = 0, const char* const* filternames = nullptr);

    template <typename T, typename U,
             typename std::enable_if<has_nametag<T>(), int>::type = 0>
    void registerProperty(PropertyContainer& props, const std::string& name, T*& prop, U& self, NicePropSetterCallback<T*, U> setter = nullptr)
    {
        static const char* hints[] = { T::name().c_str() };
        registerProperty(props, name, prop, self, setter, 0, 1, hints);
    }

    template <typename T, typename U,
             typename std::enable_if<!has_nametag<T>() && has_identifier<T>::value, int>::type = 0>
    void registerProperty(PropertyContainer& props, const std::string& name, T*& prop, U& self, NicePropSetterCallback<T*, U> setter = nullptr)
    {
        registerProperty(props, name, prop, self, setter, T::id);
    }

    template <typename T, typename U,
             typename std::enable_if<!has_identifier<T>::value, int>::type = 0>
    void registerProperty(PropertyContainer& props, const std::string& name, T*& prop, U& self, NicePropSetterCallback<T*, U> setter = nullptr)
    {
        registerProperty(props, name, prop, self, setter, 0);
    }
}

#include "PropertyContainer.hpp"

namespace gamelib
{
    template <typename T, typename U>
    void registerProperty(PropertyContainer& props, const std::string& name, T*& prop, U& self, NicePropSetterCallback<T*, U> setter, unsigned int filter, int max, const char* const* filternames)
    {
        static_assert(std::is_base_of<Component, T>(), "Not a component");
        static_assert(std::is_base_of<Component, U>(), "Callback data must be the calling component or the component to search in");
        props.registerProperty(name, prop, setter, &self, &propComponent, filter, max, filternames);
    }
}

#endif
