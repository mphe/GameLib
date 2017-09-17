#ifndef GAMELIB_ENTITYCONFIGRESOURCE_HPP
#define GAMELIB_ENTITYCONFIGRESOURCE_HPP

#include "JsonResource.hpp"

/*
 * Provides an alternative loader for JsonResource that automatically registers
 * it at EntityFactory.
 */

namespace gamelib
{
    class ResourceManager;

    void registerEntityConfigLoader(ResourceManager& resmgr);

    BaseResourceHandle entityConfigLoader(const std::string& fname, ResourceManager* resmgr);
}

#endif
