#include "gamelib/properties/PropResource.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/json/json-resources.hpp"
#include "gamelib/imgui/resources.hpp"
#include "gamelib/utils/utils.hpp"

namespace gamelib
{
    PropResource propResource;

    bool PropResource::loadFromJson(UNUSED const PropertyHandle& prop, BaseResourceHandle* ptr, const Json::Value& node) const
    {
        return ::gamelib::loadFromJson(node, ptr);
    }

    void PropResource::writeToJson(UNUSED const PropertyHandle& prop, const BaseResourceHandle* ptr, Json::Value& node)   const
    {
        ::gamelib::writeToJson(node, *ptr);
    }

    bool PropResource::drawGui(const PropertyHandle& prop, const std::string& name, BaseResourceHandle* ptr) const
    {
        return inputResource(name.c_str(), ptr, prop.id);
    }
}
