#ifndef GAMELIB_PROPERTY_HPP
#define GAMELIB_PROPERTY_HPP

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
#define PROP_METHOD_TYPE(T, F) +[](T*, T const* val, decltype(this) self) { self->F(*val); }
#define PROP_METHOD_CLASS(T, F, C) +[](T*, T const* val, C* self) { self->F(*val); }


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

            // TODO: would be nice to get rid of those resource overloads
            //       find another way of passing the expected resource ID to the prop?
            template <typename T>
            void registerProperty(const std::string& name, ResourceHandle<T>& prop,
                    const char* const* hints = nullptr);

            template <typename T, typename U>
            void registerProperty(const std::string& name, const ResourceHandle<T>& prop, NicePropSetterCallback<ResourceHandle<T>, U> setter, U* self,
                    const char* const* hints = nullptr);


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
            auto _registerProperty(const std::string& name, const void* prop, PropSetterCallback setter, void* self, const IPropType* type, int min, int max, const char* const* hints) -> void;

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
        _registerProperty(name, &prop, nullptr, data, categorizeProperty(prop), min, max, hints);
    }

    template <typename T, typename U>
    void PropertyContainer::registerProperty(const std::string& name, const T& prop, NicePropSetterCallback<T, U> setter, U* self, int min, int max, const char* const* hints)
    {
        _registerProperty(name, &prop, (PropSetterCallback)setter, static_cast<void*>(self), categorizeProperty(prop), min, max, hints);
    }

    template <typename T, typename U>
    void PropertyContainer::registerProperty(const std::string& name, T& prop, const IPropType* type, int min, int max, const char* const* hints, U* data)
    {
        _registerProperty(name, &prop, nullptr, data, type, min, max, hints);
    }

    template <typename T, typename U>
    void PropertyContainer::registerProperty(const std::string& name, const T& prop, NicePropSetterCallback<T, U> setter, U* self, const IPropType* type, int min, int max, const char* const* hints)
    {
        _registerProperty(name, &prop, (PropSetterCallback)setter, static_cast<void*>(self), type, min, max, hints);
    }

    template <typename T>
    void PropertyContainer::registerProperty(const std::string& name, ResourceHandle<T>& prop, const char* const* hints)
    {
        _registerProperty(name, &prop, nullptr, nullptr, categorizeProperty(prop), T::id, T::id, hints);
    }

    template <typename T, typename U>
    void PropertyContainer::registerProperty(const std::string& name, const ResourceHandle<T>& prop, NicePropSetterCallback<ResourceHandle<T>, U> setter, U* self, const char* const* hints)
    {
        _registerProperty(name, &prop, (PropSetterCallback)setter, static_cast<void*>(self), categorizeProperty(prop), T::id, T::id, hints);
    }
}

#endif
