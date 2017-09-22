#include "gamelib/core/res/JsonResource.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/utils/json.hpp"

namespace gamelib
{
    void registerJsonLoader(ResourceManager& resmgr)
    {
        // Maybe spr will become a separate resource
        for (auto& i : { "json", "spr" })
            resmgr.registerFileType(i, jsonLoader);
    }

    BaseResourceHandle jsonLoader(const std::string& fname, ResourceManager* resmgr)
    {
        auto json = JsonResource::create();
        if (!loadJsonFromFile(fname, *json))
            return nullptr;
        return json.as<BaseResource>();
    }
}
