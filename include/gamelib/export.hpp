#ifndef GAMELIB_EXPORT_HPP
#define GAMELIB_EXPORT_HPP

#include <string>
#include "gamelib/json/json-file.hpp"
#include "gamelib/json/json-utils.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/core/rendering/CameraSystem.hpp"
#include "gamelib/core/rendering/RenderSystem.hpp"
#include "gamelib/core/ecs/serialization.hpp"

namespace gamelib
{
    bool save(const std::string& fname);
    bool saveToJson(Json::Value& node);

    // Same as saveToJson() but save to a file
    template <typename F>
    bool save(const std::string& fname, F callback);

    // Calls a callback for each entity about to be serialized.
    // The callback modifies the generated json data (or not).
    // If it returns false, the entity will be skipped completely.
    // Signature: bool(Json::Value&, Entity&)
    template <typename F>
    bool saveToJson(Json::Value& node, F callback);


    // If direct is set, it loads entities directly as they are defined in json.
    // Otherwise, entities are created using the factory and get deltas from json.
    bool loadSave(const std::string& fname, bool direct = false);
    bool loadSaveFromJson(const Json::Value& node, bool direct = false);






    // Implementations

    template <typename F>
    bool saveToJson(Json::Value& node, F callback)
    {
        LOG("Saving game...");

        // Cache normalized entity templates
        std::unordered_map<std::string, Json::Value> norments;

        auto camsys = getSubsystem<CameraSystem>();
        if (camsys)
            camsys->writeToJson(node["cameras"]);

        auto rensys = getSubsystem<RenderSystem>();
        if (rensys)
            rensys->writeToJson(node["rendersystem"]);

        auto entmgr = getSubsystem<EntityManager>();
        if (!entmgr)
            return true;

        auto factory = getSubsystem<EntityFactory>();
        auto& mgrnode = node["entmgr"];

        entmgr->foreach([&](EntityReference ent){
            Entity& i = *ent;

            auto& name = i.getName();
            Json::Value entcfg;

            auto cacheit = norments.find(name);
            if (cacheit == norments.end())
            {
                Json::Value enttemplate;
                if (getNormalizedEntityTemplate(i, &enttemplate, *factory))
                    cacheit = norments.insert(std::make_pair(name, std::move(enttemplate))).first;
            }

            if (cacheit != norments.end())
                getConfigDelta(i, cacheit->second, &entcfg);
            else
                writeToJson(entcfg, i);

            if (callback(entcfg, i) && !entcfg.isNull())
                mgrnode.append(entcfg);

            return false;
        });

        LOG("Saving finished");
        return true;
    }

    template <typename F>
    bool save(const std::string& fname, F callback)
    {
        Json::Value node;
        if (!saveToJson(node, callback))
            return false;

        if (writeJsonToFile(fname, node))
            return true;

        LOG_ERROR("Failed to write savefile ", fname);
        return false;
    }
}

#endif
