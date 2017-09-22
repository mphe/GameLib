#ifndef GAMELIB_EXPORT_HPP
#define GAMELIB_EXPORT_HPP

#include <string>
#include "utils/json.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/components/update/QPhysics.hpp"

namespace gamelib
{
    bool saveState(const std::string& fname);
    void saveStateToJson(Json::Value& node);

    bool loadState(const std::string& fname);
    void loadStateFromJson(const Json::Value& node);

    // Calls a callback for each entity about to be serialized.
    // The callback handles the serialization (or not).
    // Signature: void(Json::Value&, Entity&)
    template <typename F>
    void saveStateToJson(Json::Value& node, F callback)
    {
        auto scene = getSubsystem<Scene>();
        if (scene)
            scene->writeToJson(node["scene"]);

        auto entmgr = getSubsystem<EntityManager>();
        if (entmgr)
            entmgr->writeToJson(node["entmgr"], callback);

        QPhysics::writeGlobalsToJson(node["physics"]);
    }

    // Calls a callback for each entity about to be serialized.
    // The callback handles the serialization (or not).
    // Signature: void(Json::Value&, Entity&)
    template <typename F>
    bool saveState(const std::string& fname, F callback)
    {
        Json::Value node;
        saveStateToJson(node, callback);
        if (writeJsonToFile(fname, node))
            return true;

        LOG_ERROR("Failed to save state to file ", fname);
        return false;
    }
}

#endif
