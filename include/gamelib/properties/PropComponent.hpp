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
            bool loadFromJson(const PropertyHandle& prop, Component** ptr, const Json::Value& node) const final override;
            void writeToJson(const PropertyHandle& prop, Component* const* ptr, Json::Value& node) const final override;
            bool drawGui(const PropertyHandle& prop, const std::string& name, Component** ptr) const final override;

            static Entity* getEntity(const PropertyHandle& prop);
    };

    extern PropComponent propComponent;

    template <typename T, typename U>
    using ComponentPropSetter = NicePropSetterCallback<T*, U>;

    namespace detail
    {
        class general_ {};
        class special_ : public general_ {};

        template <typename T>
        constexpr bool has_name(general_) { return false; };

        template <typename T, typename = decltype(T::name)>
        constexpr bool has_name(special_) { return true; };

        template <typename T>
        constexpr bool has_name() { return has_name<T>(special_()); }

        template <typename T>
        constexpr bool has_id(general_) { return false; };

        template <typename T, typename = decltype(T::id)>
        constexpr bool has_id(special_) { return true; };

        template <typename T>
        constexpr bool has_id() { return has_id<T>(special_()); }
    }


    template <typename T, typename U>
    void registerProperty(PropertyContainer& props, const std::string& name, T*& prop, U& self, NicePropSetterCallback<T*, U> setter, unsigned int filter, int max = 0, const char* const* filternames = nullptr);

    template <typename T, typename U, typename std::enable_if<detail::has_name<T>(), int>::type = 0>
    void registerProperty(PropertyContainer& props, const std::string& name, T*& prop, U& self, NicePropSetterCallback<T*, U> setter = nullptr)
    {
        static constexpr const char* hints[] = { T::name };
        registerProperty(props, name, prop, self, setter, 0, 1, hints);
    }

    template <typename T, typename U, typename std::enable_if<!detail::has_name<T>() && detail::has_id<T>(), int>::type = 0>
    void registerProperty(PropertyContainer& props, const std::string& name, T*& prop, U& self, NicePropSetterCallback<T*, U> setter = nullptr)
    {
        registerProperty(props, name, prop, self, setter, T::id);
    }

    template <typename T, typename U, typename std::enable_if<!detail::has_id<T>(), int>::type = 0>
    void registerProperty(PropertyContainer& props, const std::string& name, T*& prop, U& self, NicePropSetterCallback<T*, U> setter = nullptr)
    {
        registerProperty(props, name, prop, self, setter, 0);
    }
}

#include "Property.hpp"

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
