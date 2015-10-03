#include <SFML/Graphics.hpp>
#include "Camera.hpp"
#include "math/math.hpp"
#include "utils/log.hpp"

using namespace geometry;

namespace engine
{
    Camera::Camera() :
            targetobj(0),
            zoom(1)
    { }

    Camera::Camera(const Vector2<float>& pos, const Vector2<float>& size) :
            targetobj(0),
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
        zoom = node.get("zoom", 1).asInt();

        LOG_DEBUG(LOG_DUMP(pos.x), ", ", LOG_DUMP(pos.y));
        LOG_DEBUG(LOG_DUMP(size.x), ", ", LOG_DUMP(size.y));
        LOG_DEBUG(LOG_DUMP(zoom));

        if (size.x == 0 || size.y == 0)
            LOG_WARN("Camera size is 0");

        const auto& vp = node["viewport"];
        viewport.pos.x = vp.get("x", 0).asFloat();
        viewport.pos.y = vp.get("y", 0).asFloat();
        viewport.size.x = vp.get("w", 1).asFloat();
        viewport.size.y = vp.get("h", 1).asFloat();

        LOG_DEBUG(LOG_DUMP(viewport.pos.x), ", ", LOG_DUMP(viewport.pos.y));
        LOG_DEBUG(LOG_DUMP(viewport.size.x), ", ", LOG_DUMP(viewport.size.y));

        if (viewport.size.x == 0 || viewport.size.y == 0)
            LOG_WARN("Camera viewport size is 0");

        return true;
    }


    void Camera::update(float fps)
    {
        if (targetobj)
            center(targetobj->getPosition());
        else
            pos += _speed / fps;
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

    void Camera::center(const geometry::Vector2<float>& pos)
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

    void Camera::render(sf::RenderTarget& target) const
    {
        AABB<int> rect(getCamRect());

        sf::VertexArray va(sf::PrimitiveType::Lines, 8);
        va.append(sf::Vertex(sf::Vector2f(rect.pos.x, rect.pos.y), sf::Color::Black));
        va.append(sf::Vertex(sf::Vector2f(rect.pos.x, rect.pos.y + rect.size.y), sf::Color::Black));

        va.append(sf::Vertex(sf::Vector2f(rect.pos.x + rect.size.x, rect.pos.y), sf::Color::Black));
        va.append(sf::Vertex(sf::Vector2f(rect.pos.x + rect.size.x, rect.pos.y + rect.size.y), sf::Color::Black));

        va.append(sf::Vertex(sf::Vector2f(rect.pos.x, rect.pos.y + rect.size.y), sf::Color::Black));
        va.append(sf::Vertex(sf::Vector2f(rect.pos.x + rect.size.x, rect.pos.y + rect.size.y), sf::Color::Black));

        va.append(sf::Vertex(sf::Vector2f(rect.pos.x, rect.pos.y), sf::Color::Black));
        va.append(sf::Vertex(sf::Vector2f(rect.pos.x + rect.size.x, rect.pos.y), sf::Color::Black));

        target.draw(va);
    }


    AABB<float> Camera::getCamRect() const
    {
        return AABB<float>(pos, size * zoom);
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
        view.setCenter(pos.x + size.x / 2 * zoom, pos.y + size.y / 2 * zoom);
        view.setViewport(sf::FloatRect(viewport.pos.x, viewport.pos.y, viewport.size.x, viewport.size.y));
        return view;
    }
}
