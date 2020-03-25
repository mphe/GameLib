#ifndef GAMELIB_ENTITYCONFIGRESOURCE_HPP
#define GAMELIB_ENTITYCONFIGRESOURCE_HPP

#include "Resource.hpp"
#include "json/json.h"

namespace gamelib
{
    class ResourceManager;

    auto registerEntityConfigLoader(ResourceManager& resmgr) -> void;

    // Stores the json config node and generates a normalized version as needed.
    class EntityConfig
    {
        public:
            EntityConfig(const Json::Value& cfg);

            auto getName() const -> std::string;

            // Get entity config.
            auto getConfig() const -> const Json::Value&;

            // Get normalized entity config. Will lazy evaluate and cache for future uses.
            auto getNormalizedConfig() const -> const Json::Value&;

            // File load handler for ResourceManager
            static auto loadHandler(const std::string& fname, ResourceManager* resmgr) -> BaseResourceHandle;

        private:
            Json::Value _config;
            mutable Json::Value _normalized;
    };

    typedef Resource<EntityConfig, 0xd5b8e3bf> EntityResource;
}

#endif
