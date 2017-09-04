#include "gamelib/res/TextureResource.hpp"
#include "gamelib/res/ResourceManager.hpp"

namespace gamelib
{
    void registerTextureLoader(ResourceManager& resmgr)
    {
        for (auto& i : { "bmp", "png", "tga", "jpg", "gif", "psd", "hdr", "pic" })
            resmgr.registerFileType(i, textureLoader);
    }

    BaseResourceHandle textureLoader(const std::string& fname, ResourceManager* resmgr)
    {
        auto tex = TextureResource::create();
        if (!tex->loadFromFile(fname))
            return nullptr;
        tex->setRepeated(true);
        return tex.as<BaseResource>();
    }
}
