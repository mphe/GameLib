#ifndef GAMELIB_CAMERA_TRACKER_HPP
#define GAMELIB_CAMERA_TRACKER_HPP

#include "gamelib/components/UpdateComponent.hpp"
#include "gamelib/core/geometry/Transformable.hpp"

namespace gamelib
{
    class Camera;

    class CameraTracker : public UpdateComponent, public Transformable
    {
        public:
            ASSIGN_NAMETAG("CameraTracker");

        public:
            CameraTracker();

            auto update(float elapsed) -> void final override;

            auto shake(float seconds) -> void;
            auto shake()              -> void;

            auto getCamera() const -> Camera*;
            auto getBBox() const   -> math::AABBf final override;

            auto setSize(const math::Vec2f& size) -> void;

            auto getTransform()       -> Transformable* final override;
            auto getTransform() const -> const Transformable* final override;

        protected:
            auto _init() -> bool final override;
            auto _quit() -> void final override;
            auto _onChanged(const sf::Transform& old) -> void final override;

        public:
            int camera;
            float shakerad;
            float shakeduration;
            float shakeMultiplier;
            math::Vec2f size;
            bool centertrack;

        private:
            float _secondsleft;
            bool _shake;
    };
}

#endif
