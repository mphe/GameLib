#include "gamelib/core/res/EntityResource.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/core/ecs/serialization.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/json/json-utils.hpp"
#include "gamelib/json/json-file.hpp"
#include <set>

namespace gamelib
{
    auto registerEntityConfigLoader(ResourceManager& resmgr) -> void
    {
        for (auto& i : { "ent", "entity" })
            resmgr.registerFileType(i, EntityConfig::loadHandler);
    }


    EntityConfig::EntityConfig(const Json::Value& cfg) :
        _config(cfg)
    { }

    auto EntityConfig::getName() const -> std::string
    {
        return _config["name"].asString();
    }

    auto EntityConfig::getConfig() const -> const Json::Value&
    {
        return _config;
    }

    auto EntityConfig::getNormalizedConfig() const -> const Json::Value&
    {
        if (_normalized.isNull())
            normalizeConfig(getConfig(), &_normalized, *EntityFactory::getActive());
        return _normalized;
    }

    auto EntityConfig::loadHandler(const std::string& fname, ResourceManager* resmgr) -> BaseResourceHandle
    {
        static std::set<std::string> _cycleDetect;

        Json::Value config;

        if (!loadJsonFromFile(fname, config))
            return nullptr;

        const std::string basename = config.get("base", "").asString();
        if (!basename.empty())
        {
            const std::string name = config["name"].asString();
            if (_cycleDetect.find(name) != _cycleDetect.end())
            {
                LOG_ERROR("Cycle detected: ", name);
                return nullptr;
            }

            _cycleDetect.insert(name);
            auto baseres = resmgr->get(basename).as<EntityResource>();
            _cycleDetect.erase(name);

            if (!baseres)
            {
                LOG_ERROR("Unable to retrieve base entity");
                return nullptr;
            }

            // Make a copy of base and merge current entity into it
            Json::Value baseconfig = baseres->_config;
            mergeJson(config, &baseconfig);
            config = baseconfig;
        }

        if (config.isMember("children"))
        {
            LOG_ERROR("Can't define children in entity configs -> removing");
            config.removeMember("children");
        }

        auto res = EntityResource::create(config);
        auto factory = getSubsystem<EntityFactory>();
        if (factory)
            factory->add(res);

        return res;
    }
}
