#include "gamelib/res/predefs.hpp"
#include "gamelib/res/ResourceManager.hpp"
#include "gamelib/utils/json.hpp"

namespace gamelib
{
    void registerPredefLoaders(ResourceManager& resmgr)
    {
        for (auto& i : { "bmp", "png", "tga", "jpg", "gif", "psd", "hdr", "pic" })
            resmgr.registerFileType(i, textureLoader);

        resmgr.registerFileType("json", jsonLoader);
    }

    BaseResourceHandle textureLoader(const std::string& fname, ResourceManager* resmgr)
    {
        auto tex = TextureResource::create();
        if (!tex->loadFromFile(fname))
            return nullptr;
        tex->setRepeated(true);
        return tex.as<BaseResource>();
    }

    BaseResourceHandle jsonLoader(const std::string& fname, ResourceManager* resmgr)
    {
        auto json = JsonResource::create();
        if (!loadJsonFromFile(fname, *json))
            return nullptr;
        return json.as<BaseResource>();
    }
}
