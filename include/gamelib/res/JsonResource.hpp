#ifndef GAMELIB_JSONRESOURCE_HPP
#define GAMELIB_JSONRESOURCE_HPP

#include "Resource.hpp"
#include "json/json.h"

namespace gamelib
{
    class ResourceManager;

    typedef Resource<Json::Value, 0xdaf5902a> JsonResource;

    void registerJsonLoader(ResourceManager& resmgr);

    BaseResourceHandle jsonLoader(const std::string& fname, ResourceManager* resmgr);
}

#endif
