#include "gamelib/core/Camera.hpp"
#include "math/math.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/utils/json.hpp"

using namespace math;

namespace gamelib
{
    Camera::Camera() :
            zoom(1)
    { }

    Camera::Camera(const Vec2f& pos, const Vec2f& size) :
            zoom(1),
            pos(pos),
            size(size)
    { }


    bool Camera::loadFromJson(const Json::Value& node)
    {
        gamelib::loadFromJson(node["pos"], pos);
        gamelib::loadFromJson(node["size"], size);
        gamelib::loadFromJson(node["velocity"], vel);
        zoom = node.get("zoom", zoom).asFloat();

        if (size.x == 0 || size.y == 0)
            LOG_WARN("Camera size is 0");

        if (node.isMember("viewport"))
        {
            const auto& vp = node["viewport"];
            gamelib::loadFromJson(vp["pos"], viewport.pos);
            gamelib::loadFromJson(vp["size"], viewport.size);
        }

        if (viewport.size.x == 0 || viewport.size.y == 0)
            LOG_WARN("Camera viewport size is 0");

        return true;
    }

    void Camera::writeToJson(Json::Value& node)
    {
        gamelib::writeToJson(node["pos"], pos);
        gamelib::writeToJson(node["size"], size);
        gamelib::writeToJson(node["velocity"], vel);
        node["zoom"] = zoom;

        auto& vp = node["viewport"];
        gamelib::writeToJson(vp["pos"], viewport.pos);
        gamelib::writeToJson(vp["size"], viewport.size);
    }


    void Camera::update(float elapsed)
    {
        pos += vel * elapsed;
    }


    void Camera::setMotion(float speed, float dir)
    {
        vel.x = math::lengthdirX(speed, dir);
        vel.y = math::lengthdirY(speed, dir);
    }

    void Camera::addMotion(float speed, float dir)
    {
        vel.x += math::lengthdirX(speed, dir);
        vel.y += math::lengthdirY(speed, dir);
    }

    void Camera::center(const math::Vec2f& pos)
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

    void Camera::zoomTowards(float x, float y, float zoom_)
    {
        auto box = getCamRect();
        Vec2f diff = Vec2f(x, y) - (box.pos + box.size / 2);
        zoom += zoom_;
        pos -= diff * zoom_;
        // TODO: Fix this. It's not entirely accurate, but good enough
        // (for small values) for now.
    }

    AABBf Camera::getCamRect() const
    {
        return AABBf(pos + (size - (size * zoom)) / 2, size * zoom);
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
