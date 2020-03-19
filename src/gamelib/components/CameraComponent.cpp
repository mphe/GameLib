#include "gamelib/components/CameraComponent.hpp"
#include "gamelib/core/rendering/CameraSystem.hpp"
#include "gamelib/core/event/EventManager.hpp"
#include "gamelib/events/CameraEvents.hpp"

namespace gamelib
{
    void TriggerCameraShake_Handler(CameraComponent* self, EventPtr event)
    {
        if (!self->isInitialized())
            return;

        auto ev = event->get<TriggerCameraShake>();

        if (!ev->camname.empty() && ev->camname != self->Camera::name)
            return;

        self->shakeMultiplier = ev->multiplier;

        if (ev->duration > 0)
            self->shake(ev->duration);
        else
            self->shake();
    }


    CameraComponent::CameraComponent() :
        shakerad(5),
        shakeMultiplier(1)
    {
        _props.registerProperty("name", Camera::name);
        _props.registerProperty("active", active);
        _props.registerProperty("aspect-ratio", ratio, 0, NumRatios, str_aspectratios);
        _props.registerProperty("viewport-start", viewport.pos);
        _props.registerProperty("viewport-stop", viewport.size);
        _props.registerProperty("zoom", PROP_ACCESSOR_BUFFERED(setZoom, getZoom), this);
        _props.registerProperty("size", _size, PROP_METHOD(_size, setSize), this);
        _props.registerProperty("shakeRadius", shakerad);
        _props.registerProperty("shakeMultiplier", shakeMultiplier);
        _props.registerProperty("shakeDuration", _shaketimer.seconds);
    }

    void CameraComponent::shake(float seconds)
    {
        _shaketimer.start(seconds, false);
    }

    void CameraComponent::shake()
    {
        _shaketimer.start();
    }

    auto CameraComponent::update(float elapsed) -> void
    {
        // NOTE: Shaking uses the local transform, therefore only works as
        // long as CameraComponent has its own loca-to-entity transform.

        _shaketimer.update(elapsed);

        if (_shaketimer)
        {
            _shaketimer.stop();
            setLocalPosition(math::Point2f(0, 0));
        }
        else if (_shaketimer.isRunning())
        {
            const auto radius = shakerad * shakeMultiplier;
            math::Vec2f offset(-radius);
            offset += math::Vec2f(std::fmod(rand(), radius * 2),
                                  std::fmod(rand(), radius * 2));
            setLocalPosition(offset.asPoint());
        }
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
        auto sys = getSubsystem<CameraSystem>();
        if (sys)
            sys->add(this);
        _triggerShake = registerEvent<TriggerCameraShake>(TriggerCameraShake_Handler, this);
        return sys;
    }

    void CameraComponent::_quit()
    {
        auto sys = getSubsystem<CameraSystem>();
        if (sys)
            sys->remove(this);
        _triggerShake.unregister();
    }
}
