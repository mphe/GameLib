#include "gamelib/core/res/JsonResource.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/json/json-file.hpp"

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
