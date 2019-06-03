#ifndef GAMELIB_JSON_RESOURCES_HPP
#define GAMELIB_JSON_RESOURCES_HPP

#include "json/json.h"
#include "gamelib/core/res/Resource.hpp"

namespace gamelib
{
    class ResourceManager;

    void writeToJson(Json::Value& node, const BaseResourceHandle& handle, const ResourceManager* resmgr = nullptr);
    bool loadFromJson(const Json::Value& node, BaseResourceHandle* handle, ResourceManager* resmgr = nullptr);

    template <typename T>
    bool loadFromJson(const Json::Value& node, ResourceHandle<T>* handle, ResourceManager* resmgr = nullptr)
    {
        return loadFromJson(node, handle->asBasePtr(), resmgr);
    }
}

#endif
