#ifndef GAMELIB_CAMERACOMPONENT_HPP
#define GAMELIB_CAMERACOMPONENT_HPP

#include "gamelib/core/ecs/Component.hpp"
#include "gamelib/core/rendering/Camera.hpp"
#include "gamelib/core/event/EventHandle.hpp"
#include "gamelib/utils/Timer.hpp"

namespace gamelib
{
    class CameraComponent : public Identifier<0x02b1aab7, Component>, public Camera
    {
        public:
            ASSIGN_NAMETAG("CameraComponent");

        public:
            CameraComponent();

            auto shake(float seconds) -> void;
            auto shake()              -> void;

            auto update(float elapsed) -> void override;

            auto getTransform()       -> Transformable* override;
            auto getTransform() const -> const Transformable* override;

        protected:
            auto _init() -> bool override;
            auto _quit() -> void override;

        public:
            float shakerad;
            float shakeMultiplier;

        protected:
            Timer _shaketimer;
            EventHandle _triggerShake;
    };
}

#endif
