#ifndef GAMELIB_RESOURCE_TYPEDEFS_HPP
#define GAMELIB_RESOURCE_TYPEDEFS_HPP

#include <SFML/Graphics/Texture.hpp>
#include "Resource.hpp"
#include "json/json.h"

namespace gamelib
{
    class ResourceManager;

    typedef Resource<sf::Texture, 0x7bdbdbc2> TextureResource;
    typedef Resource<Json::Value, 0xdaf5902a> JsonResource;

    void registerPredefLoaders(ResourceManager& resmgr);

    BaseResourceHandle textureLoader(const std::string& fname, ResourceManager* resmgr);
    BaseResourceHandle jsonLoader(const std::string& fname, ResourceManager* resmgr);
}

#endif
