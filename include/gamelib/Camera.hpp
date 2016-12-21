#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "math/geometry/Vector.hpp"
#include "math/geometry/AABB.hpp"
#include "gamelib/utils/JsonObject.hpp"
#include "gamelib/Updatable.hpp"

namespace sf
{
    class View;
    class RenderTarget;
    class Transform;
}

namespace gamelib
{
    class Camera : public Json::JsonObject, public gamelib::Updatable
    {
        public:
            Camera();
            Camera(const math::Vec2f& pos, const math::Vec2f& size);

            bool loadFromJson(const Json::Value& node);

            void update(float elapsed);

            void setMotion(float speed, float dir);
            void addMotion(float speed, float dir);

            void center(float x, float y);
            void center(const math::Vec2f& pos);
            void move(float x, float y);

            math::AABB<float> getCamRect() const;
            sf::Transform getTransform() const;
            sf::View getView() const;

        public:
            float zoom;
            math::Vec2f pos;
            math::Vec2f size;
            math::AABBf viewport;

        private:
            math::Vec2f _speed;
    };
}

#endif
