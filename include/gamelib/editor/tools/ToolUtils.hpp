#ifndef EDITOR_TOOL_UTILS_HPP
#define EDITOR_TOOL_UTILS_HPP

#include <SFML/Graphics.hpp>
#include "math/geometry/Point2.hpp"
#include "math/geometry/Polygon.hpp"
#include "gamelib/core/ecs/ecsmeta.hpp"

namespace gamelib
{
    class Entity;
    class PolygonBrushComponent;

    // Half size of a drag box
    constexpr int drag_size = 2;
    constexpr int snap_distance = 3;

    bool selectDragBoxes(const math::AbstractPolygon<float>& pol, size_t* selected);
    bool checkDragBox(const math::Point2f& p);

    void drawDragBox(sf::RenderTarget& target, const math::Point2f& p, bool selected = false);
    void drawDragBox(sf::RenderTarget& target, float x, float y, bool selected = false);

    void drawLine(sf::RenderTarget& target, const math::Point2f& a, const math::Point2f& b, sf::Color color = sf::Color::White);
    void drawLine(sf::RenderTarget& target, float x1, float y1, float x2, float y2, sf::Color color = sf::Color::White);

    void drawArrow(sf::RenderTarget& target, float x1, float y1, float x2, float y2, sf::Color color = sf::Color::Green);

    void drawDragBoxes(sf::RenderTarget& target, const math::AbstractPolygon<float>& pol, size_t selected = -1);

    void drawRectOutline(sf::RenderTarget& target, const math::AABBf& box, sf::Color col = sf::Color::Magenta, const sf::Transform& transform = sf::Transform::Identity);

    void drawCollisions(sf::RenderTarget& target, const Entity& ent, unsigned int flags, sf::Color col = sf::Color::Red);
    void drawNormals(sf::RenderTarget& target, const math::AbstractPolygon<float>& pol, sf::Color col = sf::Color::Green);

    // Returns the snapped vertex or the grid snapped input point in case it wasn't snapped
    math::Point2f snap(const math::AbstractPolygon<float>& pol, const math::Point2f& p, size_t ignoreindex = -1);

    CompRef<PolygonBrushComponent> getIfBrush(EntityReference ent);
}

#endif
