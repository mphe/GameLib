#include "gamelib/components/RenderComponent.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/imgui/inputs.hpp"

namespace gamelib
{
    class PropSceneObject : public PropType<0x4933b918, SceneObject>
    {
        public:
            bool loadFromJson(const PropertyHandle& prop, SceneObject* ptr, const Json::Value& node) const
            {
                return ptr->SceneObject::loadFromJson(node);
            }

            void writeToJson(const PropertyHandle& prop, Json::Value& node) const
            {
                static_cast<SceneObject*>(prop.getMutableOrNull())->SceneObject::writeToJson(node);
                node.removeMember("transform");
            }

            bool drawGui(const PropertyHandle& prop, const std::string& name, SceneObject* ptr) const
            {
                bool changed = inputSceneData(*ptr);

                Layer::Handle handle = ptr->getLayer();
                if (inputLayer("Layer", &handle))
                {
                    ptr->setLayer(handle);
                    changed = true;
                }
                return changed;
            }
    } propSceneObject;


    RenderComponent::RenderComponent(const std::string& name) :
        Identifier(name)
    {
        _props.registerProperty("renderopts", *static_cast<SceneObject*>(this), &propSceneObject);
    }

    bool RenderComponent::_init()
    {
        auto scene = getSubsystem<Scene>();
        if (!scene)
            return false;

        scene->add(this);
        return true;
    }

    void RenderComponent::_quit()
    {
        unregister();
    }

    Transformable* RenderComponent::getTransform()
    {
        return this;
    }

    const Transformable* RenderComponent::getTransform() const
    {
        return this;
    }
}
