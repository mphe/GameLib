#include "gamelib/res/JsonResource.hpp"
#include "gamelib/res/ResourceManager.hpp"
#include "gamelib/utils/json.hpp"

namespace gamelib
{
    void registerJsonLoader(ResourceManager& resmgr)
    {
        resmgr.registerFileType("json", jsonLoader);
    }

    BaseResourceHandle jsonLoader(const std::string& fname, ResourceManager* resmgr)
    {
        auto json = JsonResource::create();
        if (!loadJsonFromFile(fname, *json))
            return nullptr;
        return json.as<BaseResource>();
    }
}
