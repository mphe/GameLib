#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "math/geometry/Vector.hpp"
#include "math/geometry/AABB.hpp"
#include "gamelib/res/JsonSerializer.hpp"
#include "gamelib/update/Updatable.hpp"
#include <SFML/Graphics.hpp>

// TODO: Consider using a sf::View internally

namespace gamelib
{
    class Camera : public gamelib::JsonSerializer, public gamelib::Updatable
    {
        public:
            Camera();
            Camera(const math::Vec2f& pos, const math::Vec2f& size);

            bool loadFromJson(const Json::Value& node);
            void writeToJson(Json::Value& node);

            void update(float elapsed);

            void setMotion(float speed, float dir);
            void addMotion(float speed, float dir);

            void center(float x, float y);
            void center(const math::Vec2f& pos);
            void move(float x, float y);

            void zoomTowards(float x, float y, float zoom);

            math::AABB<float> getCamRect() const;
            sf::Transform getTransform() const;
            sf::View getView() const;

        public:
            float zoom;
            math::Vec2f pos;
            math::Vec2f size;
            math::AABBf viewport;
            math::Vec2f vel;
    };
}

#endif
