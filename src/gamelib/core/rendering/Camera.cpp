#include "gamelib/core/rendering/Camera.hpp"
#include "gamelib/utils/conversions.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    Camera::Camera() :
        Camera("Camera")
    { }

    Camera::Camera(const std::string& name) :
        Camera(name, math::Point2f(), math::Vec2f(640, 480))
    { }

    Camera::Camera(const math::Point2f& pos, const math::Vec2f& size) :
        Camera("Camera", pos, size)
    { }

    Camera::Camera(const std::string& name, const math::Point2f& pos, const math::Vec2f& size) :
        viewport(0, 0, 1, 1),
        ratio(Fit),
        active(true),
        name(name),
        _size(size)
    {
        setPosition(pos);
        setOrigin((size / 2).asPoint());
    }


    // bool Camera::loadFromJson(const Json::Value& node)
    // {
    //     _name = node.get("name", _name).asString();
    //     gamelib::loadFromJson(node["pos"], pos);
    //     gamelib::loadFromJson(node["size"], size);
    //     gamelib::loadFromJson(node["velocity"], vel);
    //     zoom = node.get("zoom", zoom).asFloat();
    //     ratio = static_cast<AspectRatio>(node.get("ratio", ratio).asInt());
    //     active = node.get("active", active).asBool();
    //
    //     if (size.x == 0 || size.y == 0)
    //         LOG_WARN("Camera size is 0");
    //
    //     if (node.isMember("viewport"))
    //     {
    //         const auto& vp = node["viewport"];
    //         gamelib::loadFromJson(vp["pos"], viewport.pos);
    //         gamelib::loadFromJson(vp["size"], viewport.size);
    //     }
    //
    //     if (viewport.size.x == 0 || viewport.size.y == 0)
    //         LOG_WARN("Camera viewport size is 0");
    //
    //     return true;
    // }
    //
    // void Camera::writeToJson(Json::Value& node) const
    // {
    //     gamelib::writeToJson(node["pos"], pos);
    //     gamelib::writeToJson(node["size"], size);
    //     gamelib::writeToJson(node["velocity"], vel);
    //     node["zoom"] = zoom;
    //     node["ratio"] = ratio;
    //     node["active"] = active;
    //     node["name"] = getName();
    //
    //     auto& vp = node["viewport"];
    //     gamelib::writeToJson(vp["pos"], viewport.pos);
    //     gamelib::writeToJson(vp["size"], viewport.size);
    // }


    // void Camera::center(const math::Point2f& p)
    // {
    //     pos = p - size / 2;
    // }

    // void Camera::center(float x, float y)
    // {
    //     center(math::Point2f(x, y));
    // }

    auto Camera::getSize() const -> const math::Vec2f&
    {
        return _size;
    }

    auto Camera::setSize(const math::Vec2f& size) -> void
    {
        if (size.x == 0 || size.y == 0)
            LOG_WARN("Camera size is 0");
        _size = size;
        _markDirty();
    }

    auto Camera::zoom(float zoom) -> void
    {
        setZoom(getZoom() + zoom);
    }

    auto Camera::setZoom(float zoom) -> void
    {
        setScale(zoom, zoom);
    }

    auto Camera::getZoom() const -> float
    {
        if (getScale().x != getScale().y)
            LOG_DEBUG_WARN("Camera is not evenly scaled");
        return getScale().x;
    }

    void Camera::zoomTowards(float x, float y, float zoom)
    {
        zoomTowards(math::Point2f(x, y), zoom);
    }

    void Camera::zoomTowards(const math::Point2f& p, float zoom_)
    {
        auto box = getBBox();
        auto diff = p - box.getCenter();
        move(-diff * zoom_);
        zoom(zoom_);
        // TODO: Fix this. It's not entirely accurate, but good enough
        // (for small values) for now.
    }

    math::AABBf Camera::getBBox() const
    {
        return convert(getMatrix().transformRect(sf::FloatRect(
                        sf::Vector2f(), convert(_size))));
    }

    sf::View Camera::getView() const
    {
        // Can't just use getBBox() because it doesn't include rotation
        const auto pos = convert(getMatrix().transformPoint(convert(getOrigin()))) - getOrigin().asVector() * getScale();
        auto endsize = _size * getScale();
        sf::View view;
        view.setSize(convert(endsize));
        view.setCenter(convert(pos + endsize / 2));
        view.setRotation(getRotation());
        view.setViewport(convert(viewport));
        return view;
    }

    sf::View Camera::getView(const sf::RenderTarget& target) const
    {
        // Can't use the sf::View version of applyAspectRatio() because we
        // want to use the actual size and not the scaled size.
        auto view = getView();
        view.setViewport(convert(
                    applyAspectRatio(_size, viewport, convert(target.getSize()), ratio)));
        return view;
    }

    void Camera::apply(sf::RenderTarget& target) const
    {
        target.setView(getView(target));
    }
}
