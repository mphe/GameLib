#ifndef GAMELIB_RESOURCE_TYPEDEFS_HPP
#define GAMELIB_RESOURCE_TYPEDEFS_HPP

#include <SFML/Graphics/Texture.hpp>
#include "Resource.hpp"

namespace gamelib
{
    class ResourceManager;

    typedef Resource<sf::Texture, 0x7bdbdbc2> TextureResource;

    void registerPredefLoaders(ResourceManager& resmgr);

    BaseResourceHandle textureLoader(const std::string& fname, ResourceManager* resmgr);
}

#endif
