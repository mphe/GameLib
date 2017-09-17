#ifndef GAMELIB_TEXTURERESOURCE_HPP
#define GAMELIB_TEXTURERESOURCE_HPP

#include <SFML/Graphics/Texture.hpp>
#include "Resource.hpp"

namespace gamelib
{
    class ResourceManager;

    typedef Resource<sf::Texture, 0x7bdbdbc2> TextureResource;

    void registerTextureLoader(ResourceManager& resmgr);

    BaseResourceHandle textureLoader(const std::string& fname, ResourceManager* resmgr);
}

#endif
