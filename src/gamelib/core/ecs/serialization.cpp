#include "gamelib/core/ecs/serialization.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/json/json-utils.hpp"

namespace gamelib
{
    // Extracts the id and removes the id part from the string
    unsigned int extractID(std::string* name)
    {
        size_t split = name->find_last_of('#');
        unsigned int id = 1;    // 1 indexed. See also Entity::add() implementation.

        if (split != std::string::npos)
        {
            id = fromString<unsigned int>((name->substr(split + 1)), 1);
            name->erase(split);
        }
        return id;
    }

    std::string generateName(const std::string& name, unsigned int id)
    {
        return join(name, "#", id);
    }

    void getDefaultComponentConfig(const std::string& name, Json::Value* out, EntityFactory& factory)
    {
        assert(out && "out must not be null");
        auto comp = factory.createComponent(name);
        comp->init();
        // It's safe to use writeToJson even if the component does not belong to an entity
        comp->writeToJson(*out);
        comp->quit();
    }


    bool normalizeConfig(const Json::Value& node, Json::Value* out_, EntityFactory& factory)
    {
        assert(out_ && "out must not be null");
        auto& out = *out_;
        bool good = true;

        // Create default meta and merge it with custom one
        Entity ent;
        writeToJson(out, ent);
        mergeJson(node, &out);
        out.removeMember("components"); // components are handled manually

        if (!out.isMember("name") || out["name"].asString().empty())
        {
            LOG_ERROR("No entity name specified");
            good = false;
        }

        if (node.isMember("components"))
        {
            auto& comps = node["components"];

            if (!comps.isObject())
            {
                LOG_ERROR("Invalid component list");
                return false;
            }

            auto& outcomps = out["components"];

            for (auto it = comps.begin(), end = comps.end(); it != end; ++it)
            {
                std::string name = it.key().asString();
                unsigned int id = extractID(&name);
                auto cleanname = generateName(name, id);

                if (name.empty())
                {
                    LOG_ERROR("Empty component name");
                    good = false;
                    continue;
                }

                if (outcomps.isMember(cleanname))
                {
                    LOG_WARN("Multiple definitions of the same component ", cleanname, " -> Skipping ", it.key().asCString());
                    good = false;
                    continue;
                }

                if (it->isObject())
                {
                    auto& compcfg = outcomps[cleanname];
                    getDefaultComponentConfig(name, &compcfg, factory);
                    mergeJson(*it, &compcfg);
                }
                else
                {
                    // Can be tolerated and treated as a component without config.
                    LOG_WARN("Invalid config entry for component ", it.key().asCString(), " -> Treating as empty config");
                    good = false;
                    outcomps[cleanname] = Json::objectValue;
                }
            }
        }

        return good;
    }


    auto getNormalizedEntityTemplate(const Entity& ent, Json::Value* out, EntityFactory& factory) -> bool
    {
        assert(out && "out must not be null");

        const std::string& name = ent.getName();
        const Json::Value* enttemplate = factory.findEntity(name);

        if (!enttemplate)
        {
            LOG_WARN("Entity is not registered, can't compute delta config: ", name);
            return false;
        }

        normalizeConfig(*enttemplate, out, factory);
        return true;
    }

    auto getConfigDelta(const Entity& ent, Json::Value* out, EntityFactory& factory) -> bool
    {
        assert(out && "out must not be null");

        Json::Value normtemplate;
        if (!getNormalizedEntityTemplate(ent, &normtemplate, factory))
        {
            writeToJson(*out, ent);
            return false;
        }

        getConfigDelta(ent, normtemplate, out);
        return true;
    }

    auto getConfigDelta(const Entity& ent, const Json::Value& normalized, Json::Value* out) -> bool
    {
        assert(out && "out must not be null");
        Json::Value cfg;
        writeToJson(cfg, ent);
        diffJson(cfg, normalized, out);
        (*out)["name"] = ent.getName();
        return true;
    }


    bool loadFromJson(const Json::Value& node, Entity& ent)
    {
        ent.clearComponents();
        return extendFromJson(node, ent, true);
    }

    bool extendFromJson(const Json::Value& node, Entity& ent, bool createMissing)
    {
        ent._name = node.get("name", ent.getName()).asString();
        ent.flags = node.get("flags", ent.flags).asUInt();

        if (node.isMember("transform"))
            loadFromJson(node["transform"], ent.getTransform(), false);

        if (node.isMember("components"))
        {
            struct CompInfo
            {
                Component* comp;
                const Json::Value* node;
            };

            auto& comps = node["components"];
            auto factory = getSubsystem<EntityFactory>();
            std::vector<CompInfo> loadlist;

            if (!comps.isObject())
            {
                LOG_ERROR("Invalid component list in entity ", ent.getName());
                return false;
            }

            for (auto it = comps.begin(), end = comps.end(); it != end; ++it)
            {
                if (!it->isObject())
                {
                    LOG_ERROR("Invalid config entry in entity ", ent.getName(), " for component ", it.key().asString());
                    continue;
                }

                std::string name = it.key().asString();
                unsigned int id = extractID(&name);

                if (name.empty())
                {
                    LOG_ERROR("Empty component name in entity ", ent.getName());
                    continue;
                }

                // Collect component pointers and their json nodes.
                // Since components might need their entity or other components during load time,
                // one can not load them immediately after finding/creating in this loop.
                // Instead we first collect component pointers and their respective json nodes,
                // creating new components if necessary.
                // In a second step, we loop through the collected data and call loadFromJson()
                // for each component.
                // This assures that the whole component list is present and an entity is assigned
                // when a component is loaded.

                bool found = false;
                for (auto& i : ent)
                    if (i.ptr->getName() == name && i.id == id)
                    {
                        loadlist.push_back({ i.ptr.get(), &(*it) });
                        found = true;
                        break;
                    }

                // Component is already present
                if (found)
                    continue;

                if (!createMissing)
                    LOG_WARN("Couldn't find matching component in entity ", ent.getName(), " for component ", it.key().asString());
                else
                {
                    auto comp = factory->createComponent(name);
                    if (!comp)
                        LOG_ERROR("Failed to create component ", comp->getName());
                    else if (ent.add(std::move(comp)))
                    {
                        ent._components.back().id = id;
                        loadlist.push_back({ ent._components.back().ptr.get(), &(*it) });
                    }
                }
            }

            // Load configs
            // Don't remove components whose load failed, because code might rely on them
            for (auto& i : loadlist)
                if (!i.comp->loadFromJson(*i.node))
                    LOG_ERROR("Failed to load config in entity ", ent.getName(), " for component ", i.comp->getName());

            // Do another refresh after all components have their configs loaded.
            // Makes sure that components that search (in their _refresh function)
            // for another component that fits certain criteria will find it even
            // if the criteria is met only after loading.
            ent._refresh(PostLoad, nullptr);
        }

        return true;
    }

    void writeToJson(Json::Value& node, const Entity& ent)
    {
        node["name"] = ent.getName();
        node["flags"] = ent.flags;
        gamelib::writeToJson(node["transform"], ent.getTransform());

        auto& comps = node["components"];
        comps = Json::Value(Json::objectValue);

        for (const auto& i : ent)
            if (i.ptr)
                i.ptr->writeToJson(
                        comps[generateName(i.ptr->getName(), i.id)]);
    }
}
