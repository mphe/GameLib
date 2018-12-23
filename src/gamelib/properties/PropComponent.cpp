#include "gamelib/properties/PropComponent.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/ecs/Component.hpp"
#include "gamelib/core/ecs/serialization.hpp"
#include "gamelib/imgui/inputs.hpp"
#include "imgui.h"

namespace gamelib
{
    PropComponent propComponent;

    bool PropComponent::loadFromJson(const PropertyHandle& prop, Component** ptr, const Json::Value& node) const
    {
        if (!getEntity(prop))
        {
            LOG_ERROR("No entity pointer passed to component property");
            return false;
        }

        std::string name;
        unsigned int id;

        if (node.isString())
        {
            name = node.asString();
            if (name.find('#') == std::string::npos)
                LOG_WARN("Every component reference should have an ID, otherwise diffing might not work as expected!");
            id = extractID(&name);
        }
        else if (node.isArray() && node.size() >= 1)
        {
            name = node[0].asString();
            id = node[1].asUInt();
        }
        else
        {
            LOG_ERROR("Invalid component reference format");
            return false;
        }

        for (auto& i : *getEntity(prop))
        {
            if (i.ptr->getName() == name && i.id == id)
            {
                *ptr = i.ptr.get();
                return true;
            }
        }

        LOG_ERROR("Can't find component: ", generateName(name, id));
        return false;
    }

    void PropComponent::writeToJson(const PropertyHandle& prop, Component* const* ptr, Json::Value& node) const
    {
        if (!*ptr)
        {
            // Write empty string so that the diffing logic keeps working correctly
            // when the initial value is nonzero and was then changed to zero.
            // Otherwise it wouldn't find the difference and keep the nonzero state.
            node = "";
            return;
        }

        for (auto& i : *(*ptr)->getEntity())
            if (i.ptr.get() == *ptr)
            {
                node = generateName((*ptr)->getName(), i.id);
                return;
            }
    }

    bool PropComponent::drawGui(const PropertyHandle& prop, const std::string& name, Component** ptr) const
    {
        if (!getEntity(prop) || !prop.getData())
        {
            LOG_ERROR("No entity pointer passed to component property");
            return false;
        }
        return inputComponentSelect(name, ptr, *getEntity(prop), static_cast<Component*>(prop.getData()), prop.min, prop.max, prop.hints);
    }

    Entity* PropComponent::getEntity(const PropertyHandle& prop)
    {
        auto c = static_cast<Component*>(prop.getData());
        return c ? c->getEntity() : nullptr;
    }
}
