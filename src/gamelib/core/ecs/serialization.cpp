#include "gamelib/core/ecs/serialization.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"

namespace gamelib
{
    void _parseName(std::string* name, unsigned int* id)
    {
        size_t split = name->find_last_of('#');
        *id = 1;    // 1 indexed. See also Entity::add() implementation.

        if (split != std::string::npos)
        {
            *id = fromString<unsigned int>((name->substr(split + 1)), 1);
            name->erase(split);
        }
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
        {
            const auto& trans = node["transform"];
            math::Vec2f tmp;

            tmp.asPoint() = ent.getTransform().getPosition();
            loadFromJson(trans["pos"], tmp);
            ent.getTransform().setPosition(tmp.asPoint());

            tmp = ent.getTransform().getScale();
            loadFromJson(trans["scale"], tmp);
            ent.getTransform().setScale(tmp);

            ent.getTransform().setRotation(trans.get("angle", ent.getTransform().getRotation()).asFloat());
        }

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
                unsigned int id;
                _parseName(&name, &id);

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
                            if (comp->loadFromJson(*it))
                            {
                                ent.add(std::move(comp));
                                ent._components.back().id = id;
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
            });
    }
}
