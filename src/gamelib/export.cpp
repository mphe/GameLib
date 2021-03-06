#include "gamelib/export.hpp"
#include "gamelib/json/json-file.hpp"
#include "gamelib/json/json-utils.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/core/rendering/RenderSystem.hpp"
#include "gamelib/core/ecs/serialization.hpp"

namespace gamelib
{
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

        auto rensys = getSubsystem<RenderSystem>();
        if (rensys && node.isMember("rendersystem"))
            rensys->loadFromJson(node["rendersystem"]);

        loadEntityManagerFromJson(node["entmgr"], true, direct);

        LOG("Loading finished");
        return true;
    }

    bool saveToJson(Json::Value& node)
    {
        LOG("Saving game...");

        auto rensys = getSubsystem<RenderSystem>();
        if (rensys)
            rensys->writeToJson(node["rendersystem"]);

        auto entmgr = getSubsystem<EntityManager>();
        if (!entmgr)
            return true;

        writeEntityManagerToJson(&node["entmgr"]);

        LOG("Saving finished");
        return true;
    }

    bool save(const std::string& fname)
    {
        Json::Value node;
        if (!saveToJson(node))
            return false;

        if (writeJsonToFile(fname, node))
            return true;

        LOG_ERROR("Failed to write savefile ", fname);
        return false;
    }


    auto createHierachyFromJson(const Json::Value& node, bool direct) -> EntityReference
    {
        auto factory = getSubsystem<EntityFactory>();
        EntityReference ent;

        if (direct)
            ent = factory->createFromJson(node);
        else
            ent = factory->createWithDelta(node);

        if (!ent)
        {
            LOG_ERROR("Failed to create entity from json");
            return nullptr;
        }

        LOG_DEBUG("Created entity ", ent->getName());

        if (node.isMember("children"))
        {
            auto& childrennode = node["children"];
            for (auto& i : childrennode)
            {
                auto child = createHierachyFromJson(i, direct);
                if (child)
                    child->reparent(ent, false);
            }
        }

        return ent;
    }

    auto _recurseWriteHierachyToJson(Json::Value* out, const Entity& ent) -> void
    {
        auto factory = getSubsystem<EntityFactory>();
        const auto& name = ent.getName();
        auto handle = factory->findEntity(name);

        if (!getConfigDelta(ent, out, *factory))
        {
            LOG_WARN("Could not get deltas for entity: ", name);
            writeToJson(*out, ent);
        }

        const auto& children = ent.getChildren();
        if (children.empty())
            return;

        auto& childrennode = (*out)["children"];
        childrennode.resize(children.size());

        for (Json::ArrayIndex i = 0; i < children.size(); ++i)
            _recurseWriteHierachyToJson(&childrennode[i], *children[i]);
    }

    auto writeHierachyToJson(Json::Value* out, const Entity& ent) -> void
    {
        _recurseWriteHierachyToJson(out, ent);
    }


    auto writeEntityManagerToJson(Json::Value* node) -> void
    {
        auto entmgr = getSubsystem<EntityManager>();
        writeHierachyToJson(node, *entmgr->getRoot());
        *node = (*node)["children"];    // trim root node
    }

    auto loadEntityManagerFromJson(const Json::Value& node, bool clear, bool direct) -> bool
    {
        if (clear)
            getSubsystem<EntityManager>()->clear();

        if (!node.isArray())
        {
            LOG_ERROR("No valid entity array specified");
            return false;
        };

        for (auto& i : node)
            createHierachyFromJson(i, direct);

        return true;
    }
}
