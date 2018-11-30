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

    bool normalizeConfig(const Json::Value& node, Json::Value* out_, EntityFactory& factory)
    {
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
                    auto comp = factory.createComponent(name);
                    comp->writeToJson(compcfg);
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


    bool loadFromJson(const Json::Value& node, Entity& ent)
    {
        ent.clear();
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
            auto& comps = node["components"];
            auto factory = getSubsystem<EntityFactory>();

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

                bool found = false;
                for (auto& i : ent._components)
                    if (i.ptr->getName() == name && i.id == id)
                    {
                        if (!i.ptr->loadFromJson(*it))
                            LOG_ERROR("Failed to load config in entity ", ent.getName(), " for component ", it.key().asString());
                        found = true;
                        break;
                    }

                if (!found)
                {
                    if (!createMissing)
                        LOG_WARN("Couldn't find matching component in entity ", ent.getName(), " for component ", it.key().asString());
                    else
                    {
                        auto comp = factory->createComponent(name);
                        if (comp)
                        {
                            if (comp->loadFromJson(*it))
                            {
                                ent.add(std::move(comp));
                                ent._components.back().id = id;
                            }
                            else
                                LOG_ERROR("Failed to load component ", comp->getName());
                        }
                    }
                }
            }
        }

        return true;
    }

    void writeToJson(Json::Value& node, const Entity& ent)
    {
        writeToJson(node, ent, [](Json::Value& compnode, Component& comp) {
                comp.writeToJson(compnode);
                return true;
            });
    }
}
