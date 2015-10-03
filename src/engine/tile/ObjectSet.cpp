#include "engine/tile/ObjectSet.hpp"
#include "engine/utils/log.hpp"

namespace engine
{
    bool ObjectSet::loadFromJson(const Json::Value& node)
    {
        for (auto i = node.begin(); i != node.end(); ++i)
        {
            ObjectID id;
            if (i.key().isString())
                id = atoi(i.key().asCString());
            else if (i.key().isInt())
                id = i.key().asInt();
            else
            {
                LOG_ERROR("Invalid object ID: ", i.key().asString());
                continue;
            }

            LOG_DEBUG("Loading object ", id, "...");
            TilePtr ptr(TileFactory::create(id, NULL));
            if (ptr->loadFromJson(*i))
                _templates[id] = std::move(ptr);
        }
        return true;
    }

    void ObjectSet::clear()
    {
        _templates.clear();
        TileFactory::clear();
        LOG_WARN("ObjectSet cleared");
    }

    TilePtr ObjectSet::create(ObjectID id) const
    {
        // if there's a template for this object return a copy, otherwise create a new one
        auto i = _templates.find(id);
        return TilePtr(TileFactory::create(id, i == _templates.end() ? NULL : i->second.get()));
    }
}
