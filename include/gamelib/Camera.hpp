#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "math/geometry/Vector2.hpp"
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
            Camera(const math::Vector2<float>& pos, const math::Vector2<float>& size);

            bool loadFromJson(const Json::Value& node);

            void update(float elapsed);

            void setMotion(float speed, float dir);
            void addMotion(float speed, float dir);

            void center(float x, float y);
            void center(const math::Vector2<float>& pos);
            void move(float x, float y);

            math::AABB<float> getCamRect() const;
            sf::Transform getTransform() const;
            sf::View getView() const;

        public:
            float zoom;
            math::Vector2<float> pos;
            math::Vector2<float> size;
            math::AABB<float> viewport;

        private:
            math::Vector2<float> _speed;
    };
}

#endif
