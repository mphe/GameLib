#include "gamelib/core/Camera.hpp"
#include "math/math.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/utils/json.hpp"
#include "gamelib/utils/conversions.hpp"

using namespace math;

namespace gamelib
{
    Camera::Camera(const std::string& name) :
        Camera(name, math::Vec2f(), math::Vec2f())
    { }

    Camera::Camera(const std::string& name, const Vec2f& pos, const Vec2f& size) :
        zoom(1),
        pos(pos),
        size(size),
        ratio(Fit),
        _name(name)
    { }


    bool Camera::loadFromJson(const Json::Value& node)
    {
        _name =
        gamelib::loadFromJson(node["pos"], pos);
        gamelib::loadFromJson(node["size"], size);
        gamelib::loadFromJson(node["velocity"], vel);
        zoom = node.get("zoom", zoom).asFloat();
        ratio = static_cast<AspectRatio>(node.get("ratio", ratio).asInt());

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
        IDCounter::writeToJson(node);

        gamelib::writeToJson(node["pos"], pos);
        gamelib::writeToJson(node["size"], size);
        gamelib::writeToJson(node["velocity"], vel);
        node["zoom"] = zoom;
        node["ratio"] = ratio;

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

    void Camera::center(const math::Vec2f& p)
    {
        pos = p - size / 2;
    }

    void Camera::center(float x, float y)
    {
        center(math::Vec2f(x, y));
    }

    void Camera::move(float x, float y)
    {
        pos += math::Vec2f(x, y);
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
        return AABBf(pos.asPoint() + (size - (size * zoom)) / 2, size * zoom);
    }

    sf::View Camera::getView() const
    {
        sf::View view;
        view.setSize(size.x, size.y);
        view.zoom(zoom);
        view.setCenter(convert(pos + size / 2));
        view.setViewport(convert(viewport));
        return view;
    }

    sf::View Camera::getView(const sf::RenderTarget& target) const
    {
        auto view = getView();
        view.setSize(size.x, size.y);
        view = applyAspectRatio(view, target, ratio);
        view.zoom(zoom);
        return view;
    }

    void Camera::apply(sf::RenderTarget& target) const
    {
        target.setView(getView(target));
    }

    const std::string& Camera::getName() const
    {
        return _name;
    }
}
