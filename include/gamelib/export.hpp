#ifndef GAMELIB_EXPORT_HPP
#define GAMELIB_EXPORT_HPP

#include <string>
#include "gamelib/json/json-file.hpp"
#include "gamelib/json/json-utils.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/core/rendering/Scene.hpp"
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
        std::unordered_map<std::string, Json::Value> norments;

        auto scene = getSubsystem<Scene>();
        if (scene)
            scene->writeToJson(node["scene"]);

        auto entmgr = getSubsystem<EntityManager>();
        if (entmgr)
        {
            auto factory = getSubsystem<EntityFactory>();
            auto& mgrnode = node["entmgr"];

            for (auto& i : *entmgr)
            {
                Json::Value ent;
                writeToJson(ent, i);
                auto it = norments.find(i.getName());

                if (it == norments.end())
                {
                    auto ent = factory->findEntity(i.getName());
                    if (ent)
                    {
                        it = norments.insert(std::make_pair(i.getName(), Json::Value())).first;
                        normalizeConfig(*ent, &it->second, *factory);
                    }
                }

                if (it != norments.end())
                {
                    Json::Value diff;
                    diffJson(ent, it->second, &diff);
                    diff["name"] = i.getName();
                    ent = std::move(diff);
                }

                if (callback(ent, i) && !ent.isNull())
                    mgrnode.append(ent);

            }
        }

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
