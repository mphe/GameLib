#include "gamelib/export.hpp"
#include "gamelib/core/ecs/Entity.hpp"

namespace gamelib
{
    bool saveState(const std::string& fname)
    {
        return saveState(fname, [](Json::Value& node, Entity& ent) {
                ent.writeToJson(node);
            });
    }

    void saveStateToJson(Json::Value& node)
    {
        saveStateToJson(node, [](Json::Value& node, Entity& ent) {
                ent.writeToJson(node);
            });
    }


    bool loadState(const std::string& fname)
    {
        Json::Value node;
        if (!loadJsonFromFile(fname, node))
        {
            LOG_ERROR("Failed to load state from file ", fname);
            return false;
        }

        loadStateFromJson(node);
        return true;
    }

    void loadStateFromJson(const Json::Value& node)
    {
        auto scene = Scene::getActive();
        if (scene)
            scene->loadFromJson(node["scene"]);

        auto entmgr = EntityManager::getActive();
        if (entmgr)
            entmgr->loadFromJson(node["entmgr"]);
    }
}
