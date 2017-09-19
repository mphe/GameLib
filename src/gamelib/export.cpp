#include "gamelib/export.hpp"
#include "gamelib/core/ecs/Entity.hpp"

namespace gamelib
{
    void saveState(const std::string& fname)
    {
        Json::Value node;
        saveStateToJson(node);
        writeJsonToFile(fname, node);
    }

    void saveStateToJson(Json::Value& node)
    {
        saveStateToJson(node, [](Json::Value& node, Entity& ent) {
                ent.writeToJson(node);
            });
    }


    void loadState(const std::string& fname)
    {
        Json::Value node;
        loadJsonFromFile(fname, node);
        loadStateFromJson(node);
    }

    void loadStateFromJson(const Json::Value& node)
    {
        auto scene = Scene::getActive();
        if (scene)
            scene->loadFromJson(node["scene"]);

        auto entmgr = EntityManager::getActive();
        if (entmgr)
            entmgr->loadFromJson(node["entmgr"]);

        QPhysics::loadGlobalsFromJson(node["physics"]);
    }
}
