#include "gamelib/core/res/EntityConfigResource.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/json/json-utils.hpp"
#include "gamelib/core/res/JsonResource.hpp"

namespace gamelib
{
    void registerEntityConfigLoader(ResourceManager& resmgr)
    {
        for (auto& i : { "ent", "entity" })
            resmgr.registerFileType(i, entityConfigLoader);
    }

    BaseResourceHandle entityConfigLoader(const std::string& fname, ResourceManager* resmgr)
    {
        auto res = jsonLoader(fname, resmgr).as<JsonResource>();

        if (res->isMember("base"))
        {
            // loadOnce because it will be modified
            auto baseres = resmgr->loadOnce((*res)["base"].asString()).as<JsonResource>();
            mergeJson(*res, &*baseres);
            res = baseres;
        }

        auto factory = getSubsystem<EntityFactory>();
        if (factory)
            factory->add(*res.as<JsonResource>());

        return res;
    }
}
