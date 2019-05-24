#include "gamelib/export.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"

namespace gamelib
{
    bool save(const std::string& fname)
    {
        return save(fname, [](Json::Value&, Entity&) { return true; });
    }

    bool saveToJson(Json::Value& node)
    {
        return saveToJson(node, [](Json::Value&, Entity&) { return true; });
    }


    bool loadSave(const std::string& fname, bool direct)
    {
        Json::Value node;
        if (!loadJsonFromFile(fname, node))
        {
            LOG_ERROR("Failed to load save from file ", fname);
            return false;
        }

        return loadSaveFromJson(node, direct);
    }

    bool loadSaveFromJson(const Json::Value& node, bool direct)
    {
        LOG("Loading game...");

        auto camsys = getSubsystem<CameraSystem>();
        if (camsys && node.isMember("cameras"))
            camsys->loadFromJson(node["cameras"]);

        const auto& mgrnode = node["entmgr"];

        if (!mgrnode.isArray())
        {
            LOG_WARN("No valid entity array in save file");
            return true;
        };

        auto entmgr = getSubsystem<EntityManager>();
        auto factory = getSubsystem<EntityFactory>();

        if (entmgr && factory)
        {
            entmgr->clear();

            for (auto& i : mgrnode)
            {
                LOG_DEBUG("Creating entity ", i["name"].asString());

                if (direct)
                    factory->createFromJson(i);
                else
                    factory->createWithDelta(i["name"].asString(), i);
            }
        }

        LOG("Loading finished");
        return true;
    }
}
