#ifndef GAMELIB_PROPERTY_RESOURCE_HPP
#define GAMELIB_PROPERTY_RESOURCE_HPP

#include "PropType.hpp"
#include "gamelib/core/res/Resource.hpp"
#include "PropertyContainer.hpp"

namespace gamelib
{
    class PropResource : public PropType<0xd41af3a9, BaseResourceHandle>
    {
        public:
            bool loadFromJson(const PropertyHandle& prop, BaseResourceHandle* ptr, const Json::Value& node) const final override;
            void writeToJson(const PropertyHandle& prop, const BaseResourceHandle* ptr, Json::Value& node) const final override;
            bool drawGui(const PropertyHandle& prop, const std::string& name, BaseResourceHandle* ptr) const final override;
    };

    extern PropResource propResource;

    class PropertyContainer;

    template <typename T>
    void registerResourceProperty(PropertyContainer& props, const std::string& name, ResourceHandle<T>& prop,
            const char* const* hints = nullptr)
    {
        props.registerProperty(name, prop, &propResource, T::id, T::id, hints);
    }

    template <typename T, typename U>
    void registerResourceProperty(PropertyContainer& props, const std::string& name, const ResourceHandle<T>& prop,
            NicePropSetterCallback<ResourceHandle<T>, U> setter, U* self,
            const char* const* hints = nullptr)
    {
        props.registerProperty(name, prop, setter, self, &propResource, T::id, T::id, hints);
    }
}

#endif
