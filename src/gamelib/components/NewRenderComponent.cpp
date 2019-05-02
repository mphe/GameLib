#include "gamelib/components/NewRenderComponent.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/rendering/RenderSystem.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/imgui/inputs.hpp"
#include "imgui.h"

namespace gamelib
{
    class PropNodeHandle : public PropType<0xe56155ea, NodeHandle>
    {
        public:
            bool loadFromJson(const PropertyHandle& prop, NodeHandle* ptr, const Json::Value& cfgnode) const final override
            {
                auto self = static_cast<NewRenderComponent*>(prop.getData());
                auto sys = self->_system;
                const SceneNode& node = *sys->getNode(*ptr);

                // TODO: blendmode, layer
                auto depth = cfgnode.get("depth", node.depth).asInt();
                auto flags = cfgnode.get("flags", node.options.flags).asUInt();
                auto parallax = cfgnode.get("parallax", node.options.parallax).asFloat();

                sys->setNodeOptions(*ptr, &flags, &parallax);
                sys->setNodeDepth(*ptr, depth);

                return true;
            }

            void writeToJson(const PropertyHandle& prop, const NodeHandle* ptr, Json::Value& cfgnode) const final override
            {
                auto self = static_cast<NewRenderComponent*>(prop.getData());
                const SceneNode& node = *self->_system->getNode(*ptr);
                // TODO: blendmode, layer
                cfgnode["depth"] = node.depth;
                cfgnode["flags"] = node.options.flags;
                cfgnode["parallax"] = node.options.parallax;
            }

            bool drawGui(const PropertyHandle& prop, const std::string&, NodeHandle* ptr) const final override
            {
                auto self = static_cast<NewRenderComponent*>(prop.getData());
                auto sys = self->_system;
                const SceneNode& node = *sys->getNode(*ptr);

                int depth = node.depth;
                float parallax = node.options.parallax;
                unsigned int flags = node.options.flags;
                bool changed = false;

                if (inputBitflags("Render flags", &flags, num_renderflags, str_renderflags))
                    changed = true;

                if (ImGui::InputFloat("Parallax", &parallax, 0.01, 0.1, 3))
                    changed = true;

                // TODO: blendmode, layer

                if (changed)
                    sys->setNodeOptions(*ptr, &flags, &parallax);

                if (ImGui::InputInt("Depth", &depth, 1, 100))
                {
                    sys->setNodeDepth(*ptr, depth);
                    changed = true;
                }

                return changed;
            }
    } propNodeHandle;



    NewRenderComponent::NewRenderComponent() :
        _system(nullptr)
    { }

    NewRenderComponent::NewRenderComponent(const std::string& name) :
        Identifier(name),
        _system(nullptr)
    { }

    NewRenderComponent::~NewRenderComponent()
    {
        quit();
    }

    auto NewRenderComponent::_init() -> bool
    {
        _system = getSubsystem<RenderSystem>();
        if (!_system)
            return false;

        _handle = _system->createNode(this);

        _props.registerProperty("renderopts", _handle, +[](NodeHandle*, const NodeHandle*, NewRenderComponent*)
                {
                    LOG_ERROR("Can't set 'renderopts' property directly");
                }, this, &propNodeHandle);

        return true;
    }

    auto NewRenderComponent::_quit() -> void
    {
        _system->removeNode(_handle);
        _handle.reset();
        _props.unregisterProperty("renderopts");
    }

    auto NewRenderComponent::getTransform() -> Transformable*
    {
        return this;
    }

    auto NewRenderComponent::getTransform() const -> const Transformable*
    {
        return this;
    }

	auto NewRenderComponent::_onChanged(const sf::Transform&) -> void
	{
        _system->setNodeTransform(_handle, getMatrix());
	}

    auto NewRenderComponent::getBBox() const -> const math::AABBf&
    {
        return *_system->getNodeGlobalBBox(_handle);
    }
}
