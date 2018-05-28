#ifndef GAMELIB_PROPERTY_HPP
#define GAMELIB_PROPERTY_HPP

#include <string>
#include <unordered_map>
#include "math/geometry/Vector.hpp"
#include "gamelib/core/res/TextureResource.hpp"
#include "gamelib/core/res/JsonSerializer.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    enum PropertyType
    {
        PropInt,
        PropBitflags,
        PropFloat,
        PropString,
        PropBool,
        PropVec2i,
        PropVec2f,
        PropTexResource,
        PropUnknown
    };


    typedef void(*PropSetterCallback)(void* var, const void* value, void* self);


    class PropertyHandle
    {
        public:
            PropertyHandle();
            PropertyHandle(void* var, PropSetterCallback setter, void* self);

            auto isPrimitive() const -> bool;
            auto isVector() const    -> bool;
            auto isSetter() const    -> bool;

            auto get() const              -> const void*;
            auto getMutableOrNull() const -> void*;

            template <typename T>
            auto getAs() const -> const T&
            {
                return *static_cast<const T*>(get());
            }

            template <typename T>
            auto set(const T& val) const -> void
            {
                if (isSetter())
                    _setter(_ptr, &val, _self);
                else
                    *static_cast<T*>(_ptr) = val;
            }

        public:
            // Meta data for auto serialization, etc.
            PropertyType type;
            const char* const* hints;
            int min, max;

        private:
            void* _ptr;                  // Pointer to the variable
            PropSetterCallback _setter;  // Setter function if direct access isn't possible
            void* _self;                 // Data pointer that will be passed to the setter
    };


    class PropertyContainer : public JsonSerializer
    {
        template <typename T, typename U>
        using NicePropSetterCallback = void(*)(T* var, const T* value, U* self);

        public:
            typedef std::unordered_map<std::string, PropertyHandle> PropertyMap;

        public:
            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

            auto unregisterProperty(const std::string& name) -> void;

            template <typename T>
            void registerProperty(const std::string& name, T& prop, int min = 0, int max = 0, const char* const* hints = nullptr)
            {
                _registerProperty(name, &prop, nullptr, nullptr, categorizeProperty(prop), min, max, hints);
            }

            template <typename T>
            void registerProperty(const std::string& name, T& prop, PropSetterCallback setter, void* self, int min = 0, int max = 0, const char* const* hints = nullptr)
            {
                _registerProperty(name, &prop, setter, self, categorizeProperty(prop), min, max, hints);
            }

            template <typename T, typename U>
            void registerProperty(const std::string& name, T& prop, NicePropSetterCallback<T, U> setter, U* self, int min = 0, int max = 0, const char* const* hints = nullptr)
            {
                _registerProperty(name, &prop, (PropSetterCallback)setter, static_cast<void*>(self), categorizeProperty(prop), min, max, hints);
            }

            auto begin() const -> PropertyMap::const_iterator;
            auto begin()       -> PropertyMap::iterator;
            auto end() const   -> PropertyMap::const_iterator;
            auto end()         -> PropertyMap::iterator;

            auto size() const -> size_t;
            auto clear()      -> void;

            auto find(const std::string& name) const -> const PropertyHandle*;
            auto get(const std::string& name) const  -> const void*;

            template <typename T>
            const T* getAs(const std::string& name) const
            {
                auto ptr = find(name);
                return ptr ? &ptr->getAs<T>() : nullptr;
            }

            template <typename T>
            void set(const std::string& name, const T& val) const
            {
                auto handle = find(name);
                if (handle)
                    handle->set(val);
                else
                    LOG_WARN("Unknown property: ", name);
            }

            template <typename T>
            PropertyType categorizeProperty(const T& prop) const
            {
                if (std::is_same<T, int>())
                    return PropInt;
                else if (std::is_same<T, unsigned int>())
                    return PropBitflags;
                else if (std::is_same<T, bool>())
                    return PropBool;
                else if (std::is_same<T, float>())
                    return PropFloat;
                else if (std::is_same<T, std::string>())
                    return PropString;
                else if (std::is_same<T, math::Vec2f>())
                    return PropVec2f;
                else if (std::is_same<T, math::Vec2i>())
                    return PropVec2i;
                else if (std::is_same<T, TextureResource::Handle>())
                    return PropTexResource;
                else
                    return PropUnknown;
            }

        private:
            auto _registerProperty(const std::string& name, void* prop, PropSetterCallback setter, void* self, PropertyType type, int min, int max, const char* const* hints) -> void;

        private:
            PropertyMap _properties;
    };
}

#endif
