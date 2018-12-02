#ifndef GAMELIB_CAMERACOMPONENT_HPP
#define GAMELIB_CAMERACOMPONENT_HPP

#include "gamelib/core/ecs/Component.hpp"
#include "gamelib/core/geometry/TransformableAABB.hpp"
#include "gamelib/core/rendering/Camera.hpp"

namespace gamelib
{
    class CameraComponent : public Identifier<0x02b1aab7, Component>, public TransformableAABB
    {
        public:
            static constexpr const char* name = "CameraComponent";

        public:
            CameraComponent();
            CameraComponent(const std::string& name);
            virtual ~CameraComponent() {};

            virtual auto setSize(const math::Vec2f& size) -> void;

            auto getTransform()       -> Transformable*;
            auto getTransform() const -> const Transformable*;

        protected:
            virtual auto _onChanged(const sf::Transform& old) -> void;
            virtual auto _init() -> bool;
            virtual auto _quit() -> void;

        protected:
            Camera _cam;
    };
}

#endif
