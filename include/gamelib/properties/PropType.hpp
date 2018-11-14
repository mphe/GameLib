#ifndef GAMELIB_PROPERTY_SERIALIZER_HPP
#define GAMELIB_PROPERTY_SERIALIZER_HPP

#include "gamelib/core/Identifier.hpp"
#include "json/json.h"
#include "PropertyHandle.hpp"


namespace gamelib
{
    // class PropertyHandle;

    // Base class for serialization callbacks.
    // Don't derive directly from this except you know what you do.
    // Use PropType (or BasePropType) instead, as it automatically adds
    // some boilerplate code that makes your life way easier.
    class IPropType : public Identifiable
    {
        public:
            virtual ~IPropType() {};

            virtual auto loadFromJson(const PropertyHandle& prop, const Json::Value& node) const -> bool = 0;
            virtual auto writeToJson(const PropertyHandle& prop, Json::Value& node)        const -> void = 0;
            virtual auto drawGui(const PropertyHandle& prop, const std::string& name)      const -> bool = 0;
    };


    // Provides a buffering mechanic so that the user always gets a pointer
    // to the variable without having to worry about setters.
    // Useful for GUI stuff, as imgui always needs pointers.
    template <typename T>
    class BasePropType : public IPropType
    {
        public:
            virtual ~BasePropType() {};

            virtual auto loadFromJson(const PropertyHandle& prop, T* ptr, const Json::Value& node) const -> bool = 0;
            virtual auto writeToJson(const PropertyHandle& prop, Json::Value& node)                const -> void = 0;
            virtual auto drawGui(const PropertyHandle& prop, const std::string& name, T* ptr)      const -> bool = 0;

        private:
            virtual auto loadFromJson(const PropertyHandle& prop, const Json::Value& node) const -> bool;
            virtual auto drawGui(const PropertyHandle& prop, const std::string& name) const -> bool;
    };


    // Syntax sugar. No need to inherit from Identifier manually
    template <ID idval, typename T>
    using PropType = Identifier<idval, BasePropType<T>>;




    // Implementation

    template <typename T>
    bool BasePropType<T>::loadFromJson(const PropertyHandle& prop, const Json::Value& node) const
    {
        if (!prop.isSetter())
            return loadFromJson(prop, static_cast<T*>(prop.getMutableOrNull()), node);

        // Make a temporary value and handle to provide a pointer that can be accessed without
        // worrying if the property needs a setter.
        T tmp = prop.getAs<T>();
        PropertyHandle tmphandle(&tmp, nullptr, nullptr);
        tmphandle.hints = prop.hints;
        tmphandle.serializer = prop.serializer;
        tmphandle.min = prop.min;
        tmphandle.max = prop.max;

        if (loadFromJson(tmphandle, &tmp, node))
        {
            prop.set(tmp);
            return true;
        }
        return false;
    }

    template <typename T>
    bool BasePropType<T>::drawGui(const PropertyHandle& prop, const std::string& name) const
    {
        if (!prop.isSetter())
            return drawGui(prop, name, static_cast<T*>(prop.getMutableOrNull()));

        // Same as in loadFromJson
        T tmp = prop.getAs<T>();
        PropertyHandle tmphandle(&tmp, nullptr, nullptr);
        tmphandle.hints = prop.hints;
        tmphandle.serializer = prop.serializer;
        tmphandle.min = prop.min;
        tmphandle.max = prop.max;

        if (drawGui(tmphandle, name, &tmp))
        {
            prop.set(tmp);
            return true;
        }
        return false;
    }
}

#endif
