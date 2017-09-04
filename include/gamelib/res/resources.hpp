#ifndef GAMELIB_PREDEF_RESOURCES_HPP
#define GAMELIB_PREDEF_RESOURCES_HPP

#include "TextureResource.hpp"
#include "JsonResource.hpp"
#include "EntityConfigResource.hpp"

namespace gamelib
{
    class ResourceManager;

    void registerPredefLoaders();
    void registerPredefLoaders(ResourceManager& resmgr);
}

#endif
