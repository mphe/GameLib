#ifndef GAMELIB_PROPERTY_HPP
#define GAMELIB_PROPERTY_HPP

#include <string>
#include <unordered_map>
#include "math/geometry/Vector.hpp"
#include "gamelib/core/res/TextureResource.hpp"
#include "gamelib/core/res/JsonSerializer.hpp"

namespace gamelib
{
    enum PropertyType
    {
        PropInt,
        PropFloat,
        PropDouble,
        PropString,
        PropBool,
        PropVec2i,
        PropVec2f,
        // PropVec2d,
        PropTexResource,
        PropUnknown
    };


    class PropertyHandle
    {
        public:
            PropertyHandle();
            auto isPrimitive() const -> bool;
            auto isVector() const    -> bool;

            template <typename T>
            T& getAs()
            {
                return *static_cast<T*>(ptr);
            }

            template <typename T>
            const T& getAs() const
            {
                return *static_cast<T*>(ptr);
            }

        public:
            void* ptr;
            PropertyType type;
            const char** hints;
            int min, max;
    };


    class PropertyContainer : public JsonSerializer
    {
        public:
            typedef std::unordered_map<std::string, PropertyHandle> PropertyMap;

        public:
            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

            auto registerProperty(const std::string& name, void* prop, PropertyType type, int min = 0, int max = 0, const char** hints = nullptr) -> void;
            auto unregisterProperty(const std::string& name) -> void;

            template <typename T>
            auto registerProperty(const std::string& name, T& prop, PropertyType type, int min = 0, int max = 0, const char** hints = nullptr) -> void
            {
                registerProperty(name, &prop, type, min, max, hints);
            }

            template <typename T>
            auto registerProperty(const std::string& name, T& prop, int min = 0, int max = 0, const char** hints = nullptr) -> void
            {
                PropertyType type;

                if (std::is_same<T, int>())
                    type = PropInt;
                else if (std::is_same<T, bool>())
                    type = PropBool;
                else if (std::is_same<T, float>())
                    type = PropFloat;
                else if (std::is_same<T, double>())
                    type = PropDouble;
                else if (std::is_same<T, std::string>())
                    type = PropString;
                else if (std::is_same<T, math::Vec2f>())
                    type = PropVec2f;
                else if (std::is_same<T, math::Vec2i>())
                    type = PropVec2i;
                // else if (std::is_same<T, math::Vec2d>())
                //     type = PropVec2d;
                else if (std::is_same<T, TextureResource::Handle>())
                    type = PropTexResource;
                else
                    type = PropUnknown;

                registerProperty(name, prop, type, min, max, hints);
            }

            auto begin() const -> PropertyMap::const_iterator;
            auto begin()       -> PropertyMap::iterator;
            auto end() const   -> PropertyMap::const_iterator;
            auto end()         -> PropertyMap::iterator;

            auto find(const std::string& name) const -> const PropertyHandle*;
            auto find(const std::string& name)       -> PropertyHandle*;

            auto size() const -> size_t;
            auto clear()      -> void;

            auto get(const std::string& name)       -> void*;
            auto get(const std::string& name) const -> const void*;

            template <typename T>
            T* getAs(const std::string& name)
            {
                auto ptr = find(name);
                return ptr ? &ptr->getAs<T>() : nullptr;
            }

            template <typename T>
            const T* getAs(const std::string& name) const
            {
                auto ptr = find(name);
                return ptr ? &ptr->getAs<T>() : nullptr;
            }

        private:
            PropertyMap _properties;
    };
}

#endif
