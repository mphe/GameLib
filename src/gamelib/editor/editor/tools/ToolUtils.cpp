#include "gamelib/editor/editor/tools/ToolUtils.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/editor/editor/EditorShared.hpp"
#include "editor/components/BrushComponent.hpp"
#include "gamelib/components/geometry/AABB.hpp"
#include "gamelib/core/geometry/flags.hpp"

namespace gamelib
{
    void drawDragBox(sf::RenderTarget& target, const math::Point2f p, bool selected)
    {
        drawDragBox(target, p.x, p.y, selected);
    }

    void drawDragBox(sf::RenderTarget& target, float x, float y, bool selected)
    {
        sf::RectangleShape dragger(sf::Vector2f(drag_size * 2, drag_size * 2));
        dragger.setOutlineColor(sf::Color::Black);
        dragger.setOutlineThickness(0.5);
        dragger.setFillColor(selected ? sf::Color::Red : sf::Color::White);
        dragger.setPosition(x - drag_size, y - drag_size);
        target.draw(dragger);
    }

    void drawLine(sf::RenderTarget& target, const math::Point2f a, const math::Point2f b, sf::Color color)
    {
        drawLine(target, a.x, a.y, b.x, b.y);
    }

    void drawLine(sf::RenderTarget& target, float x1, float y1, float x2, float y2, sf::Color color)
    {
        sf::Vertex v[] = {
            sf::Vertex(sf::Vector2f(x1, y1), color),
            sf::Vertex(sf::Vector2f(x2, y2), color),
        };
        target.draw(v, 2, sf::Lines);
    }

    void drawDragBoxes(sf::RenderTarget& target, const math::Polygon<float>& pol, size_t selected)
    {
        for (size_t i = 0; i < pol.size(); ++i)
        {
            math::Point2f p(pol.get(i));

            if (i == selected)
                drawDragBox(target, p, true);
            else
                drawDragBox(target, p);
        }
    }

    void drawRectOutline(sf::RenderTarget& target, const math::AABBf& box, sf::Color col)
    {
        sf::Vertex v[] = {
            sf::Vertex(sf::Vector2f(box.x, box.y), col),
            sf::Vertex(sf::Vector2f(box.x + box.w, box.y), col),
            sf::Vertex(sf::Vector2f(box.x + box.w, box.y + box.h), col),
            sf::Vertex(sf::Vector2f(box.x, box.y + box.h), col),
            sf::Vertex(sf::Vector2f(box.x, box.y), col)
        };
        target.draw(v, 5, sf::LineStrip);
    }

    void drawCollisions(sf::RenderTarget& target, const Entity& ent, unsigned int flags, sf::Color col)
    {
        sf::VertexArray vertices(sf::Lines);

        ent.findAllByType<CollisionComponent>([&](CollisionComponent* comp) {
                if (comp->flags & flags)
                {
                    if (comp->getName() == Polygon::name && !(comp->flags & collision_noprecise))
                    {
                        auto pol = static_cast<Polygon*>(comp);
                        pol->polygon.foreachSegment([&](const math::Line2f seg) {
                                vertices.append(sf::Vertex(sf::Vector2f(seg.p.x, seg.p.y), sf::Color::Red));
                                vertices.append(sf::Vertex(sf::Vector2f(seg.p.x + seg.d.x, seg.p.y + seg.d.y), sf::Color::Red));
                                return false;
                            });
                    }
                    else if (comp->getName() == AABB::name)
                    {
                        auto& rect = comp->getBBox();
                        vertices.append(sf::Vertex(sf::Vector2f(rect.x, rect.y), sf::Color::Red));
                        vertices.append(sf::Vertex(sf::Vector2f(rect.x + rect.w, rect.y), sf::Color::Red));

                        vertices.append(sf::Vertex(sf::Vector2f(rect.x + rect.w, rect.y), sf::Color::Red));
                        vertices.append(sf::Vertex(sf::Vector2f(rect.x + rect.w, rect.y + rect.h), sf::Color::Red));

                        vertices.append(sf::Vertex(sf::Vector2f(rect.x, rect.y), sf::Color::Red));
                        vertices.append(sf::Vertex(sf::Vector2f(rect.x, rect.y + rect.h), sf::Color::Red));

                        vertices.append(sf::Vertex(sf::Vector2f(rect.x, rect.y + rect.h), sf::Color::Red));
                        vertices.append(sf::Vertex(sf::Vector2f(rect.x + rect.w, rect.y + rect.h), sf::Color::Red));
                    }
                }
                return false;
            });

        target.draw(vertices);
    }

    math::Point2f snap(const math::Polygon<float>& pol, const math::Point2f& p, size_t ignoreindex)
    {
        for (size_t i = 0; i < pol.size(); ++i)
            if (i != ignoreindex)
            {
                auto p2 = pol.get(i);
                if ((p2 - p).abs_sqr() < snap_distance * snap_distance)
                    return p2;
            }
        return EditorShared::snap(p);
    }

    BrushComponent* getIfBrush(Entity* ent)
    {
        if (ent)
            return ent->findByType<BrushComponent>();
        return nullptr;
    }
}
