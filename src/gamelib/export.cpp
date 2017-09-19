#include "gamelib/export.hpp"
#include "gamelib/core/ecs/Entity.hpp"

namespace gamelib
{
    void saveState(const std::string& fname)
    {
        Json::Value node;
        saveState(node);
        writeJsonToFile(fname, node);
    }

    void saveState(Json::Value& node)
    {
        saveState(node, [](Json::Value& node, Entity& ent) {
                ent.writeToJson(node);
            });
    }


    void loadState(const std::string& fname)
    {
        Json::Value node;
        loadJsonFromFile(fname, node);
        loadState(node);
    }

    void loadState(const Json::Value& node)
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
