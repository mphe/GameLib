#include <SFML/Graphics.hpp>
#include "gamelib/Camera.hpp"
#include "math/math.hpp"
#include "gamelib/utils/log.hpp"

using namespace math;

namespace gamelib
{
    Camera::Camera() :
            zoom(1)
    { }

    Camera::Camera(const Vector2<float>& pos, const Vector2<float>& size) :
            zoom(1),
            pos(pos),
            size(size)
    { }


    bool Camera::loadFromJson(const Json::Value& node)
    {
        pos.x = node.get("x", 0).asFloat();
        pos.y = node.get("y", 0).asFloat();
        size.x = node.get("w", 0).asInt();
        size.y = node.get("h", 0).asInt();
        zoom = node.get("zoom", 1).asFloat();

        if (size.x == 0 || size.y == 0)
            LOG_WARN("Camera size is 0");

        const auto& vp = node["viewport"];
        viewport.pos.x = vp.get("x", 0).asFloat();
        viewport.pos.y = vp.get("y", 0).asFloat();
        viewport.size.x = vp.get("w", 1).asFloat();
        viewport.size.y = vp.get("h", 1).asFloat();

        if (viewport.size.x == 0 || viewport.size.y == 0)
            LOG_WARN("Camera viewport size is 0");

        return true;
    }


    void Camera::update(float elapsed)
    {
        pos += _speed * elapsed;
    }


    void Camera::setMotion(float speed, float dir)
    {
        _speed.x = math::lengthdirX(speed, dir);
        _speed.y = math::lengthdirY(speed, dir);
    }

    void Camera::addMotion(float speed, float dir)
    {
        _speed.x += math::lengthdirX(speed, dir);
        _speed.y += math::lengthdirY(speed, dir);
    }

    void Camera::center(const math::Vector2<float>& pos)
    {
        center(pos.x, pos.y);
    }

    void Camera::center(float x, float y)
    {
        pos.x = x - size.x / 2;
        pos.y = y - size.y / 2;
    }

    void Camera::move(float x, float y)
    {
        pos.x += x;
        pos.y += y;
    }

    AABB<float> Camera::getCamRect() const
    {
        return AABB<float>(pos + (size - (size * zoom)) / 2, size * zoom);
    }

    sf::Transform Camera::getTransform() const
    {
        sf::Transform trans;
        trans.scale(zoom, zoom);
        trans.scale(viewport.size.x / size.x, viewport.size.y / size.y);
        trans.translate(viewport.pos.x - pos.x, viewport.pos.y - pos.y);
        return trans;
    }

    sf::View Camera::getView() const
    {
        sf::View view;
        view.setSize(size.x, size.y);
        view.zoom(zoom);
        view.setCenter(pos.x + size.x / 2, pos.y + size.y / 2);
        view.setViewport(sf::FloatRect(viewport.pos.x, viewport.pos.y, viewport.size.x, viewport.size.y));
        return view;
    }
}
