#include "gamelib/components/CameraComponent.hpp"
#include "gamelib/core/rendering/Scene.hpp"

namespace gamelib
{
    constexpr const char* CameraComponent::name;

    CameraComponent::CameraComponent() :
        CameraComponent("New Camera")
    { }

    CameraComponent::CameraComponent(const std::string& name_) :
        Identifier(name),
        _cam(name_)
    {
        _props.registerProperty("zoom", _cam.zoom);
        _props.registerProperty("size", _cam.size, PROP_METHOD(math::Vec2f, setSize), this);
        _props.registerProperty("aspect-ratio", _cam.ratio, 0, NumRatios, str_aspectratios);
        _props.registerProperty("viewport-start", _cam.viewport.pos);
        _props.registerProperty("viewport-stop", _cam.viewport.size);
        // TODO: name
    }

    void CameraComponent::_onChanged(const sf::Transform& old)
    {
        TransformableAABB::_onChanged(old);
        _cam.pos = getBBox().pos;
        _cam.size = getBBox().size;
    }

    void CameraComponent::setSize(const math::Vec2f& size)
    {
        _cam.size = size;
        TransformableAABB::setSize(size);
    }

    Transformable* CameraComponent::getTransform()
    {
        return this;
    }

    const Transformable* CameraComponent::getTransform() const
    {
        return this;
    }

    bool CameraComponent::_init()
    {
        auto scene = getSubsystem<Scene>();
        if (scene)
            scene->addCamera(&_cam);
        return scene;
    }

    void CameraComponent::_quit()
    {
        auto scene = getSubsystem<Scene>();
        if (scene)
            scene->removeCamera(&_cam);
    }

}
