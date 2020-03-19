#ifndef GAMELIB_PROPERTY_CONTAINER_HPP
#define GAMELIB_PROPERTY_CONTAINER_HPP

#include <string>
#include <unordered_map>
#include "gamelib/json/JsonSerializer.hpp"
#include "gamelib/utils/log.hpp"
#include "PropertyHandle.hpp"
#include "basictypes.hpp"

// Helper for registering properties with member function setters that take
// exactly the new value as argument,
// Usage: props.registerProperty("size", _size, PROP_METHOD(_size, setSize), this);
#define PROP_METHOD(T, F) PROP_METHOD_TYPE(decltype(T), F)
#define PROP_METHOD_TYPE(T, F) PROP_METHOD_CLASS(T, F, std::remove_pointer_t<decltype(this)>)
// #define PROP_METHOD_TYPE(T, F) +[](T const* val, decltype(this) self) { self->F(*val); }
#define PROP_METHOD_CLASS(T, F, C) +[](T const* val, C* self) { self->F(*val); }

// Helpers for registering properties that don't have direct variable access,
// but use getter and setter functions.
// The setter takes exactly the new value as argument.
// The getter takes no arguments and returns a const reference (const T&).
// If the getter returns a copy, use PROP_ACCESSOR_BUFFERED.
// Other getter return values are not supported by these macros.
// Usage:
//     props.registerProperty("size", PROP_ACCESSOR(setSize, getSize), this);
//     props.registerProperty("somefloat", PROP_ACCESSOR_BUFFERED(setFloat, getFloat), this);
#define PROP_ACCESSOR(S, G) \
    PROP_ACCESSOR_TYPES(S, G, std::remove_pointer_t<decltype(this)>, \
            std::remove_reference_t<std::remove_pointer_t<decltype(G())>>)

// Same as PROP_ACCESSOR but for getter functions that return a copy instead of a const reference.
#define PROP_ACCESSOR_BUFFERED(S, G) \
    PROP_ACCESSOR_TYPES_BUFFERED(S, G, std::remove_pointer_t<decltype(this)>, \
            std::remove_reference_t<std::remove_pointer_t<decltype(G())>>)

#define PROP_ACCESSOR_TYPES_BUFFERED(S, G, C, T) \
+[](T const* val, C* self) -> T const* { \
    static T tmp; \
    if (val) \
        self->S(*val); \
    tmp = self->G(); \
    return &tmp; \
}

#define PROP_ACCESSOR_TYPES(S, G, C, T) \
+[](T const* val, C* self) -> T const* { \
    if (val) \
        self->S(*val); \
    return &self->G(); \
}


        

namespace gamelib
{
    class PropertyContainer : public JsonSerializer
    {
        public:
            typedef std::unordered_map<std::string, PropertyHandle> PropertyMap;

        public:
            auto loadFromJson(const Json::Value& node) -> bool final override;
            auto writeToJson(Json::Value& node) const  -> void final override;

            auto unregisterProperty(const std::string& name) -> void;

            template <typename T, typename U = void>
            void registerProperty(const std::string& name, T& prop,
                    int min = 0, int max = 0, const char* const* hints = nullptr, U* data = nullptr);

            template <typename T, typename U>
            void registerProperty(const std::string& name, const T& prop, NicePropSetterCallback<T, U> setter, U* self,
                    int min = 0, int max = 0, const char* const* hints = nullptr);

            template <typename T, typename U = void>
            void registerProperty(const std::string& name, T& prop, const IPropType* type,
                    int min = 0, int max = 0, const char* const* hints = nullptr, U* data = nullptr);

            template <typename T, typename U>
            void registerProperty(const std::string& name, const T& prop, NicePropSetterCallback<T, U> setter, U* self, const IPropType* type,
                    int min = 0, int max = 0, const char* const* hints = nullptr);

            template <typename T, typename U>
            void registerProperty(const std::string& name, NicePropAccessorCallback<T, U> accessor, U* self,
                    int min = 0, int max = 0, const char* const* hints = nullptr);

            template <typename T, typename U>
            void registerProperty(const std::string& name, NicePropAccessorCallback<T, U> accessor, U* self, const IPropType* type,
                    int min = 0, int max = 0, const char* const* hints = nullptr);

            auto begin() const -> PropertyMap::const_iterator;
            auto begin()       -> PropertyMap::iterator;
            auto end() const   -> PropertyMap::const_iterator;
            auto end()         -> PropertyMap::iterator;

            auto size() const -> size_t;
            auto clear()      -> void;

            auto find(const std::string& name) const -> const PropertyHandle*;
            auto find(const std::string& name)       -> PropertyHandle*;
            auto get(const std::string& name) const  -> const void*;

            template <typename T>
            auto getAs(const std::string& name) const -> const T*;

            template <typename T>
            auto set(const std::string& name, const T& val) const -> void;

        private:
            auto _registerProperty(const std::string& name, const void* constprop, void* prop,
                    PropSetterCallback setter, PropAccessorCallback accessor, void* self,
                    const IPropType* type, int min, int max, const char* const* hints)
                -> void;

        private:
            PropertyMap _properties;
    };



    // Implementation
    template <typename T>
    const T* PropertyContainer::getAs(const std::string& name) const
    {
        auto ptr = find(name);
        return ptr ? &ptr->getAs<T>() : nullptr;
    }

    template <typename T>
    void PropertyContainer::set(const std::string& name, const T& val) const
    {
        auto handle = find(name);
        if (handle)
            handle->set(val);
        else
            LOG_WARN("Unknown property: ", name);
    }

    template <typename T, typename U>
    void PropertyContainer::registerProperty(const std::string& name, T& prop, int min, int max, const char* const* hints, U* data)
    {
        _registerProperty(name, nullptr, &prop, nullptr, nullptr, data, categorizeProperty(prop), min, max, hints);
    }

    template <typename T, typename U>
    void PropertyContainer::registerProperty(const std::string& name, T& prop, const IPropType* type, int min, int max, const char* const* hints, U* data)
    {
        _registerProperty(name, nullptr, &prop, nullptr, nullptr, data, type, min, max, hints);
    }

    template <typename T, typename U>
    void PropertyContainer::registerProperty(const std::string& name, const T& prop, NicePropSetterCallback<T, U> setter, U* self, int min, int max, const char* const* hints)
    {
        _registerProperty(name, &prop, nullptr, (PropSetterCallback)setter, nullptr, self, categorizeProperty(prop), min, max, hints);
    }

    template <typename T, typename U>
    void PropertyContainer::registerProperty(const std::string& name, const T& prop, NicePropSetterCallback<T, U> setter, U* self, const IPropType* type, int min, int max, const char* const* hints)
    {
        _registerProperty(name, &prop, nullptr, (PropSetterCallback)setter, nullptr, self, type, min, max, hints);
    }

    template <typename T, typename U>
    void PropertyContainer::registerProperty(const std::string& name, NicePropAccessorCallback<T, U> accessor, U* self,
            int min, int max, const char* const* hints)
    {
        const T* emptyptr = nullptr;    // passed to categorizeProperty()
        _registerProperty(name, nullptr, nullptr, nullptr, (PropAccessorCallback)accessor, self, categorizeProperty(*emptyptr), min, max, hints);
    }

    template <typename T, typename U>
    void PropertyContainer::registerProperty(const std::string& name, NicePropAccessorCallback<T, U> accessor, U* self, const IPropType* type,
            int min, int max, const char* const* hints)
    {
        _registerProperty(name, nullptr, nullptr, nullptr, (PropAccessorCallback)accessor, self, type, min, max, hints);
    }
}

#endif
