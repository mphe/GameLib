#include "gamelib/core/res/SoundResource.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/utils/utils.hpp"

namespace gamelib
{
    void registerSoundLoader(ResourceManager& resmgr)
    {
        for (auto& i : { "wav", "ogg", "flac" })
            resmgr.registerFileType(i, soundLoader);
    }

    BaseResourceHandle soundLoader(const std::string& fname, UNUSED ResourceManager* resmgr)
    {
        auto sound = SoundResource::create();
        if (!sound->loadFromFile(fname))
            return nullptr;
        return sound.as<BaseResource>();
    }
}
