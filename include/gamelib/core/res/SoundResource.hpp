#ifndef GAMELIB_SOUNDRERESOURCE_HPP
#define GAMELIB_SOUNDRERESOURCE_HPP

#include <SFML/Audio.hpp>
#include "Resource.hpp"

namespace gamelib
{
    class ResourceManager;

    typedef Resource<sf::SoundBuffer, 0x0f1027a9> SoundResource;

    void registerSoundLoader(ResourceManager& resmgr);

    BaseResourceHandle soundLoader(const std::string& fname, ResourceManager* resmgr);
}

#endif
