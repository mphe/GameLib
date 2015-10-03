#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "math/geometry/Vector2.hpp"
#include "math/geometry/AABB.hpp"
#include "utils/JsonObject.hpp"

namespace sf
{
    class View;
    class RenderTarget;
    class Transform;
}

namespace engine
{
    class CameraTarget
    {
        public:
            virtual ~CameraTarget() {};
            virtual geometry::Vector2<float> getPosition() const = 0;
    };


    class Camera : public Json::JsonObject
    {
        public:
            Camera();
            Camera(const geometry::Vector2<float>& pos, const geometry::Vector2<float>& size);

            using JsonObject::loadFromJson;
            bool loadFromJson(const Json::Value& node);

            void update(float fps);

            void setMotion(float speed, float dir);
            void addMotion(float speed, float dir);

            void center(float x, float y);
            void center(const geometry::Vector2<float>& pos);
            void move(float x, float y);

            void render(sf::RenderTarget& surface) const; //Renders the cam rect

            geometry::AABB<float> getCamRect() const;
            sf::Transform getTransform() const;
            sf::View getView() const;

        public:
            float zoom;
            geometry::Vector2<float> pos;
            geometry::Vector2<int> size;
            geometry::AABB<float> viewport;
            CameraTarget* targetobj;

        private:
            geometry::Vector2<float> _speed;
    };
}

#endif
