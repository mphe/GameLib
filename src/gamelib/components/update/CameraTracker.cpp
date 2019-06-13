#include "gamelib/components/update/CameraTracker.hpp"
#include "gamelib/core/rendering/CameraSystem.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/utils/conversions.hpp"
#include "gamelib/core/event/EventManager.hpp"
#include "gamelib/events/CameraEvents.hpp"

namespace gamelib
{
    void TriggerCameraShake_Handler(CameraTracker* self, EventPtr event)
    {
        auto ev = event->get<TriggerCameraShake>();

        auto cam = self->getCamera();
        if (!cam)
            return;

        if (!ev->camname.empty() && ev->camname != cam->getName())
            return;

        self->shakeMultiplier = ev->multiplier;

        if (ev->duration > 0)
            self->shake(ev->duration);
        else
            self->shake();
    }


    CameraTracker::CameraTracker() :
        UpdateComponent(1, UpdateHookType::PostPostFrame),
        camera(0),
        shakerad(5),
        shakeduration(0.25),
        shakeMultiplier(1),
        size(320, 240),
        centertrack(true),
        _shake(false)
    {
        _props.registerProperty("shakeradius", shakerad);
        _props.registerProperty("shakeduration", shakeduration);
        _props.registerProperty("shakeMultiplier", shakeMultiplier);
        _props.registerProperty("camera", camera);
        _props.registerProperty("centertrack", centertrack);
        _props.registerProperty("size", size, PROP_METHOD(size, setSize), this);
    }

    void CameraTracker::update(float elapsed)
    {
        auto ent = getEntity();
        if (!ent)
            return;

        Camera* cam = getCamera();
        if (!cam)
            return;

        math::Vec2f offset;

        if (_shake)
        {
            _secondsleft -= elapsed;
            if (_secondsleft <= 0)
            {
                _shake = false;
                return;
            }

            auto radius = shakerad * shakeMultiplier;
            offset.fill(-radius);
            offset += math::Vec2f(std::fmod(rand(), radius * 2),
                                  std::fmod(rand(), radius * 2));
        }

        if (centertrack)
            cam->center(ent->getTransform().getPosition() + offset);
        else
            cam->pos = getPosition() + offset;
    }

    auto CameraTracker::setSize(const math::Vec2f& size) -> void
    {
        this->size = size;

        auto cam = getCamera();
        if (cam)
        {
            if (!centertrack)
                cam->pos = getPosition();
            cam->size = size * getScale();
            _markDirty();
        }
    }

    void CameraTracker::shake(float seconds)
    {
        _shake = true;
        _secondsleft = seconds;
    }

    void CameraTracker::shake()
    {
        shake(shakeduration);
    }

    Camera* CameraTracker::getCamera() const
    {
        return getSubsystem<CameraSystem>()->get(camera);
    }

    auto CameraTracker::getBBox() const -> math::AABBf
    {
        auto cam = getCamera();
        if (cam)
            return cam->getCamRect();
        return math::AABBf();
    }

    auto CameraTracker::_init() -> bool
    {
        if (!UpdateComponent::_init())
            return false;

        auto cam = getCamera();
        if (cam)
            size = cam->size;

        registerEvent<TriggerCameraShake>(TriggerCameraShake_Handler, this);

        return true;
    }

    auto CameraTracker::_quit() -> void
    {
        UpdateComponent::_quit();
        unregisterEvent<TriggerCameraShake>(TriggerCameraShake_Handler, this);
    }

    auto CameraTracker::_onChanged(const sf::Transform& old) -> void
    {
        setSize(size);
    }

    auto CameraTracker::getTransform() -> Transformable*
    {
        return this;
    }

    auto CameraTracker::getTransform() const -> const Transformable*
    {
        return this;
    }
}
