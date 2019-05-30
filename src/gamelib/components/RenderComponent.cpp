#include "gamelib/components/RenderComponent.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/rendering/RenderSystem.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/imgui/inputs.hpp"
#include "gamelib/json/json-rendering.hpp"
#include "imgui.h"

namespace gamelib
{
    class PropNodeHandle : public PropType<0xe56155ea, NodeHandle>
    {
        public:
            bool loadFromJson(const PropertyHandle& prop, NodeHandle* ptr, const Json::Value& cfgnode) const final override
            {
                auto self = static_cast<RenderComponent*>(prop.getData());
                auto sys = self->_system;
                const RenderNode& node = *sys->getNode(*ptr);

                auto options = node.options;
                ::gamelib::loadFromJson(cfgnode, &options);
                sys->setNodeOptions(*ptr, options);

                auto depth = cfgnode.get("depth", node.depth).asInt();
                sys->setNodeDepth(*ptr, depth);

                if (cfgnode.isMember("layer"))
                {
                    auto name = cfgnode["layer"].asString();
                    if (!name.empty())
                    {
                        auto layer = sys->findLayer(name);
                        if (layer)
                            sys->setNodeLayer(*ptr, layer);
                    }
                }

                return true;
            }

            void writeToJson(const PropertyHandle& prop, const NodeHandle* ptr, Json::Value& cfgnode) const final override
            {
                auto self = static_cast<RenderComponent*>(prop.getData());
                const RenderNode& node = *self->_system->getNode(*ptr);
                // TODO: blendmode, layer
                cfgnode["depth"] = node.depth;
                cfgnode["flags"] = node.options.flags;
                cfgnode["parallax"] = node.options.parallax;

                const auto* layer = self->_system->getLayer(node.layer);
                cfgnode["layer"] = !layer ? "" : layer->name;
            }

            bool drawGui(const PropertyHandle& prop, const std::string&, NodeHandle* ptr) const final override
            {
                auto self = static_cast<RenderComponent*>(prop.getData());
                auto sys = self->_system;
                const RenderNode& node = *sys->getNode(*ptr);

                LayerHandle handle = node.layer;
                int depth = node.depth;
                float parallax = node.options.parallax;
                unsigned int flags = node.options.flags;
                bool changed = false;

                changed |= inputBitflags("Render flags", &flags, num_renderflags, str_renderflags);
                changed |= ImGui::InputFloat("Parallax", &parallax, 0.01, 0.1, 3);

                if (changed)
                    sys->setNodeOptions(*ptr, &flags, &parallax);

                // TODO: blendmode

                if (inputLayer("Layer", &handle, sys))
                {
                    sys->setNodeLayer(*ptr, handle);
                    changed = true;
                }

                if (ImGui::InputInt("Depth", &depth, 1, 100))
                {
                    sys->setNodeDepth(*ptr, depth);
                    changed = true;
                }

                return changed;
            }
    } propNodeHandle;



    RenderComponent::RenderComponent() :
        _system(nullptr)
    { }

    RenderComponent::RenderComponent(const std::string& name) :
        Identifier(name),
        _system(nullptr)
    { }

    RenderComponent::~RenderComponent()
    {
        quit();
    }

    auto RenderComponent::_init() -> bool
    {
        _system = getSubsystem<RenderSystem>();
        if (!_system)
            return false;

        _handle = _system->createNode(this);

        _props.registerProperty("renderopts", _handle,
                +[](NodeHandle* var, const NodeHandle* val, RenderComponent*)
                {
                    if (*var != *val)
                        LOG_ERROR("Can't set 'renderopts' property directly");
                }, this, &propNodeHandle);

        return true;
    }

    auto RenderComponent::_quit() -> void
    {
        _system->removeNode(_handle);
        _handle.reset();
        _props.unregisterProperty("renderopts");
    }

    auto RenderComponent::getTransform() -> Transformable*
    {
        return this;
    }

    auto RenderComponent::getTransform() const -> const Transformable*
    {
        return this;
    }

	auto RenderComponent::_onChanged(const sf::Transform&) -> void
	{
        _system->setNodeTransform(_handle, getMatrix());
	}

    auto RenderComponent::getBBox() const -> const math::AABBf&
    {
        return *_system->getNodeGlobalBBox(_handle);
    }
}
