#ifndef GAMELIB_PROPCOMPONENT_HPP
#define GAMELIB_PROPCOMPONENT_HPP

#include "PropType.hpp"

namespace gamelib
{
    class Component;
    class Entity;
    class PropertyContainer;

    class PropComponent : public PropType<0xe73dee69, Component*>
    {
        public:
            bool loadFromJson(const PropertyHandle& prop, Component** ptr, const Json::Value& node) const;
            void writeToJson(const PropertyHandle& prop, Component* const* ptr, Json::Value& node) const;
            bool drawGui(const PropertyHandle& prop, const std::string& name, Component** ptr) const;

            static Entity* getEntity(const PropertyHandle& prop);
    };

    extern PropComponent propComponent;

    template <typename T, typename U>
    using ComponentPropSetter = NicePropSetterCallback<T*, U>;

    namespace detail
    {
        template <typename T, typename = int>
        struct has_name : std::false_type { };

        template <typename T>
        struct has_name<T, decltype((void) T::name, 0)> : std::true_type { };
    }


    template <typename T, typename U>
    void registerProperty(PropertyContainer& props, const std::string& name, T*& prop, U& self, ComponentPropSetter<T, U> setter, unsigned int filter, int max = 0, const char* const* filternames = nullptr);

    template <typename T, typename U, typename std::enable_if<detail::has_name<T>::value, int>::type = 0>
    void registerProperty(PropertyContainer& props, const std::string& name, T*& prop, U& self, ComponentPropSetter<T, U> setter = nullptr)
    {
        static constexpr const char* hints[] = { T::name };
        registerProperty(props, name, prop, self, setter, 0, 1, hints);
    }

    template <typename T, typename U, typename std::enable_if<!detail::has_name<T>::value, int>::type = 0>
    void registerProperty(PropertyContainer& props, const std::string& name, T*& prop, U& self, ComponentPropSetter<T, U> setter = nullptr)
    {
        registerProperty(props, name, prop, self, setter, T::id);
    }
}

#include "Property.hpp"

namespace gamelib
{
    template <typename T, typename U>
    void registerProperty(PropertyContainer& props, const std::string& name, T*& prop, U& self, ComponentPropSetter<T, U> setter, unsigned int filter, int max, const char* const* filternames)
    {
        static_assert(std::is_base_of<Component, T>(), "Not a component");
        static_assert(std::is_base_of<Component, U>(), "Callback data must be the calling component or the component to search in");
        props.registerProperty(name, prop, setter, &self, &propComponent, filter, max, filternames);
    }
}

#endif
