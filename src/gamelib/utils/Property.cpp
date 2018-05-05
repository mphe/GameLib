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
        return type == PropInt || type == PropFloat || type == PropDouble ||
            type == PropBool;
    }

    bool PropertyHandle::isVector() const
    {
        return type == PropVec2f || type == PropVec2i;
            // type == PropVec2d ||
    }

    bool PropertyHandle::isSetter() const
    {
        return _setter != nullptr;
    }


    const void* PropertyHandle::get() const
    {
        return _ptr;
    }




    bool PropertyContainer::loadFromJson(const Json::Value& node)
    {
        for (auto& i : _properties)
        {
            if (!node.isMember(i.first))
            {
                LOG_WARN("Missing property: ", i.first);
                continue;
            }

            const auto& propnode = node[i.first];

            switch (i.second.type)
            {
                case PropInt:
                    i.second.set<int>(propnode.asInt());
                    break;
                case PropFloat:
                    i.second.set<float>(propnode.asFloat());
                    break;
                case PropDouble:
                    i.second.set<double>(propnode.asDouble());
                    break;
                case PropString:
                    i.second.set<std::string>(propnode.asString());
                    break;
                case PropBool:
                    i.second.set<bool>(propnode.asBool());
                    break;
                case PropVec2i:
                    {
                        // TODO: Vec2i serialization is not implemented, so just use floats for now
                        // TODO: implement Vec2i serialization
                        math::Vec2f tmpvec;
                        ::gamelib::loadFromJson(propnode, tmpvec);
                        i.second.set<math::Vec2i>(tmpvec);
                        break;
                    }
                case PropVec2f:
                    {
                        math::Vec2f tmpvec;
                        ::gamelib::loadFromJson(propnode, tmpvec);
                        i.second.set<math::Vec2f>(tmpvec);
                    }
                    break;
                // case PropVec2d:
                //     ::gamelib::loadFromJson(propnode, i.second.getAs<math::Vec2d>());
                    // break;
                case PropTexResource:
                    i.second.set<TextureResource::Handle>(ResourceManager::getActive()->get(propnode.asString()));
                    break;
                case PropUnknown:
                    LOG_WARN("Can't read unknown property: ", i.first);
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
                case PropFloat:
                    propnode = i.second.getAs<float>();
                    break;
                case PropDouble:
                    propnode = i.second.getAs<double>();
                    break;
                case PropString:
                    propnode = i.second.getAs<std::string>();
                    break;
                case PropBool:
                    propnode = i.second.getAs<bool>();
                    break;
                case PropVec2i:
                    ::gamelib::writeToJson(propnode, i.second.getAs<math::Vec2i>());
                    break;
                case PropVec2f:
                    ::gamelib::writeToJson(propnode, i.second.getAs<math::Vec2f>());
                    break;
                // case PropVec2d:
                //     ::gamelib::writeToJson(propnode, i.second.getAs<math::Vec2d>());
                //     break;
                case PropTexResource:
                    propnode = i.second.getAs<TextureResource::Handle>().getResource()->getPath();
                    break;
                case PropUnknown:
                    LOG_WARN("Can't write unknown property: ", i.first);
                    break;
            }
        }
    }


    void PropertyContainer::_registerProperty(const std::string& name, void* prop, PropSetterCallback setter, void* self, PropertyType type, int min, int max, const char** hints)
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
