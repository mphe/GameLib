#include "gamelib/utils/Property.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/utils/json.hpp"
#include "gamelib/core/res/ResourceManager.hpp"

namespace gamelib
{
    PropertyHandle::PropertyHandle() :
        PropertyHandle(nullptr, nullptr, nullptr)
    { }

    PropertyHandle::PropertyHandle(void* var, PropSetterCallback setter, void* self) :
        type(PropUnknown),
        hints(nullptr),
        min(0),
        max(0),
        _ptr(var),
        _setter(setter),
        _self(self)
    { }

    bool PropertyHandle::isPrimitive() const
    {
        return type == PropInt || type == PropFloat ||
            type == PropBool || type == PropBitflags;
    }

    bool PropertyHandle::isVector() const
    {
        return type == PropVec2f || type == PropVec2i;
    }

    bool PropertyHandle::isSetter() const
    {
        return _setter != nullptr;
    }


    const void* PropertyHandle::get() const
    {
        return _ptr;
    }

    void* PropertyHandle::getMutableOrNull() const
    {
        return isSetter() ? nullptr : _ptr;
    }




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

            switch (prop->type)
            {
                case PropInt:
                    prop->set<int>(it->asInt());
                    break;
                case PropBitflags:
                    prop->set<unsigned int>(it->asUInt());
                    break;
                case PropFloat:
                    prop->set<float>(it->asFloat());
                    break;
                case PropString:
                    prop->set<std::string>(it->asString());
                    break;
                case PropBool:
                    prop->set<bool>(it->asBool());
                    break;
                case PropVec2i:
                    {
                        // TODO: Vec2i serialization is not implemented, so just use floats for now
                        // TODO: implement Vec2i serialization
                        math::Vec2f tmpvec;
                        gamelib::loadFromJson(*it, tmpvec);
                        prop->set<math::Vec2i>(tmpvec);
                        break;
                    }
                case PropVec2f:
                    {
                        math::Vec2f tmpvec;
                        gamelib::loadFromJson(*it, tmpvec);
                        prop->set<math::Vec2f>(tmpvec);
                    }
                    break;
                case PropColor:
                    {
                        math::Vec4f tmpvec;
                        gamelib::loadFromJson(*it, tmpvec);
                        prop->set<sf::Color>(sf::Color(tmpvec.r, tmpvec.g, tmpvec.b, tmpvec.a));
                    }
                    break;
                case PropResource:
                    prop->set<BaseResourceHandle>(getSubsystem<ResourceManager>()->get(it->asString()));
                    break;
                case PropUnknown:
                    LOG_WARN("Can't read unknown property: ", it.key().asCString());
                    break;
            }
        }

        return true;
    }

    void PropertyContainer::writeToJson(Json::Value& node)
    {
        for (auto& i : _properties)
        {
            auto& propnode = node[i.first];

            switch (i.second.type)
            {
                case PropInt:
                    propnode = i.second.getAs<int>();
                    break;
                case PropBitflags:
                    propnode = i.second.getAs<unsigned int>();
                    break;
                case PropFloat:
                    propnode = i.second.getAs<float>();
                    break;
                case PropString:
                    propnode = i.second.getAs<std::string>();
                    break;
                case PropBool:
                    propnode = i.second.getAs<bool>();
                    break;
                case PropVec2i:
                    ::gamelib::writeToJson(propnode, static_cast<math::Vec2f>(i.second.getAs<math::Vec2i>()));
                    break;
                case PropVec2f:
                    ::gamelib::writeToJson(propnode, i.second.getAs<math::Vec2f>());
                    break;
                case PropColor:
                    {
                        auto& col = i.second.getAs<sf::Color>();
                        ::gamelib::writeToJson(propnode, math::Vec4f(col.r, col.g, col.b, col.a));
                        break;
                    }
                case PropResource:
                    {
                        auto handle = i.second.getAs<BaseResourceHandle>();
                        if (handle)
                            propnode = handle.getResource()->getPath();
                        break;
                    }
                case PropUnknown:
                    LOG_WARN("Can't write unknown property: ", i.first);
                    break;
            }
        }
    }


    void PropertyContainer::_registerProperty(const std::string& name, void* prop, PropSetterCallback setter, void* self, PropertyType type, int min, int max, const char* const* hints)
    {
        if (_properties.find(name) != _properties.end())
            LOG_WARN("Overwriting existing property: ", name);

        auto& handle = _properties[name];
        handle = PropertyHandle(prop, setter, self);
        handle.type = type;
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
