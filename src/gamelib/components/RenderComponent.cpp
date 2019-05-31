#include "gamelib/components/RenderComponent.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/rendering/RenderSystem.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/imgui/inputs.hpp"
#include "gamelib/json/json-rendering.hpp"
#include "imgui.h"

namespace gamelib
{
    class PropLayerHandle : public PropType<0x3cf15fc5, LayerHandle>
    {
        public:
            bool loadFromJson(const PropertyHandle& prop, LayerHandle* ptr, const Json::Value& node) const final override
            {
                // maybe allow passing a pointer
                auto sys = getSubsystem<RenderSystem>();

                auto name = node.asString();
                if (!name.empty())
                    *ptr = sys->findLayer(name);
                return true;
            }

            void writeToJson(const PropertyHandle& prop, const LayerHandle* ptr, Json::Value& node) const final override
            {
                // maybe allow passing a pointer
                auto sys = getSubsystem<RenderSystem>();

                const auto* layer = sys->getLayer(*ptr);
                node = !layer ? "" : layer->name;
            }

            bool drawGui(const PropertyHandle& prop, const std::string& name, LayerHandle* ptr) const final override
            {
                // maybe allow passing a pointer
                auto sys = getSubsystem<RenderSystem>();

                return inputLayer(name.c_str(), ptr, sys);
            }
    } propLayerHandle;

    class PropNodeHandle : public PropType<0xe56155ea, NodeHandle>
    {
        public:
            bool loadFromJson(const PropertyHandle& prop, NodeHandle* ptr, const Json::Value& cfgnode) const final override
            {
                auto self = static_cast<RenderComponent*>(prop.getData());
                auto options = self->_getNode()->options;
                if (!::gamelib::loadFromJson(cfgnode, &options))
                    return false;
                self->_system->setNodeOptions(*ptr, options);
                return true;
            }

            void writeToJson(const PropertyHandle& prop, const NodeHandle* ptr, Json::Value& cfgnode) const final override
            {
                auto self = static_cast<RenderComponent*>(prop.getData());
                ::gamelib::writeToJson(cfgnode, self->_getNode()->options);
            }

            bool drawGui(const PropertyHandle& prop, const std::string& name, NodeHandle* ptr) const final override
            {
                auto self = static_cast<RenderComponent*>(prop.getData());
                RenderOptions options = self->_getNode()->options;

                if (!inputRenderOptions(name.c_str(), &options))
                    return false;

                self->_system->setNodeOptions(*ptr, options);
                return true;
            }
    } propNodeHandle;



    RenderComponent::RenderComponent() :
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

        auto depthcb = +[](const int* val, RenderComponent* self) -> const int*
        {
            if (val)
                self->setDepth(*val);
            return &self->_getNode()->depth;
        };

        auto layercb = +[](const LayerHandle* val, RenderComponent* self) -> const LayerHandle*
        {
            if (val)
                self->setLayer(*val);
            return &self->_getNode()->layer;
        };

        _props.registerProperty("depth", depthcb, this);
        _props.registerProperty("layer", layercb, this, &propLayerHandle);
        _props.registerProperty("options", _handle,
                +[](const NodeHandle* val, RenderComponent* self)
                {
                    if (self->_handle != *val)
                        LOG_ERROR("Can't set 'options' property directly");
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

    auto RenderComponent::getBBox() const -> math::AABBf
    {
        return _system->getNodeGlobalBBox(_handle);
    }

    auto RenderComponent::_getNode() const -> const RenderNode*
    {
        return _system->getNode(_handle);
    }

    auto RenderComponent::setDepth(int depth) -> void
    {
        _system->setNodeDepth(_handle, depth);
    }

    auto RenderComponent::getDepth() const -> int
    {
        return _getNode()->depth;
    }

    auto RenderComponent::setLayer(LayerHandle layer) -> void
    {
        _system->setNodeLayer(_handle, layer);
    }

    auto RenderComponent::getLayer() const -> LayerHandle
    {
        return _getNode()->layer;
    }
}
