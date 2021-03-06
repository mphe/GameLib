#include "gamelib/properties/PropertyContainer.hpp"

namespace gamelib
{
    bool PropertyContainer::loadFromJson(const Json::Value& node)
    {
        for (auto it = node.begin(), end = node.end(); it != end; ++it)
        {
            auto prop = find(it.key().asCString());

            if (!prop)
            {
                LOG_DEBUG_WARN("Property has no effect: ", it.key().asCString());
                continue;
            }

            if (prop->serializer)
            {
                if (!prop->serializer->loadFromJson(*prop, *it))
                    LOG_ERROR("Failed to load property: ", it.key().asCString());
            }
            else
                LOG_DEBUG_WARN("Property has no serializer: ", it.key().asCString());
        }

        return true;
    }

    void PropertyContainer::writeToJson(Json::Value& node) const
    {
        for (auto& i : _properties)
            if (i.second.serializer)
            {
                Json::Value& cfg = node[i.first];
                i.second.serializer->writeToJson(i.second, cfg);
                if (cfg.isNull())
                    LOG_DEBUG_WARN("Property json is null: ", i.first);
            }
            else
                LOG_DEBUG_WARN("Property has no serializer: ", i.first);
    }

    void PropertyContainer::_registerProperty(const std::string& name, const void* constprop, void* prop,
            PropSetterCallback setter, PropAccessorCallback accessor, void* data,
            const IPropType* type, int min, int max, const char* const* hints)
    {
        if (_properties.find(name) != _properties.end())
            LOG_WARN("Overwriting existing property: ", name);

        auto& handle = _properties[name];
        if (setter)
            handle = PropertyHandle(constprop, setter, data);
        else if (accessor)
            handle = PropertyHandle(accessor, data);
        else
            handle = PropertyHandle(prop, data);

        handle.serializer = type;
        handle.min = min;
        handle.max = max;
        handle.hints = hints;
    }

    void PropertyContainer::unregisterProperty(const std::string& name)
    {
        auto it = _properties.find(name);
        if (it != _properties.end())
            _properties.erase(it);
    }


    PropertyContainer::PropertyMap::const_iterator PropertyContainer::begin() const
    {
        return _properties.begin();
    }

    PropertyContainer::PropertyMap::iterator PropertyContainer::begin()
    {
        return _properties.begin();
    }

    PropertyContainer::PropertyMap::const_iterator PropertyContainer::end() const
    {
        return _properties.end();
    }

    PropertyContainer::PropertyMap::iterator PropertyContainer::end()
    {
        return _properties.end();
    }


    const PropertyHandle* PropertyContainer::find(const std::string& name) const
    {
        auto it = _properties.find(name);
        if (it != _properties.end())
            return &it->second;
        return nullptr;
    }

    PropertyHandle* PropertyContainer::find(const std::string& name)
    {
        return const_cast<PropertyHandle*>(static_cast<const PropertyContainer*>(this)->find(name));
    }

    const void* PropertyContainer::get(const std::string& name) const
    {
        auto handle = find(name);
        return handle ? handle->get() : nullptr;
    }


    size_t PropertyContainer::size() const
    {
        return _properties.size();
    }

    void PropertyContainer::clear()
    {
        _properties.clear();
    }
}
