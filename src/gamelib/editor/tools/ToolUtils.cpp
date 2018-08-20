#include "gamelib/editor/tools/ToolUtils.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/geometry/flags.hpp"
#include "gamelib/utils/conversions.hpp"
#include "gamelib/components/geometry/AABB.hpp"
#include "gamelib/components/geometry/Polygon.hpp"
#include "gamelib/components/editor/BrushComponent.hpp"

namespace gamelib
{
    bool selectDragBoxes(const math::Polygon<float>& pol, size_t* selected)
    {
        *selected = -1;
        for (size_t i = 0; i < pol.size(); ++i)
            if (checkDragBox(pol.get(i)))
            {
                *selected = i;
                return true;
            }
        return false;
    }

    bool checkDragBox(const math::Point2f& p)
    {
        static const math::AABBf drag_rect(-drag_size, -drag_size, drag_size * 2, drag_size * 2);
        return drag_rect.contains(EditorShared::getMouse() - p.asVector());
    }

    void drawDragBox(sf::RenderTarget& target, const math::Point2f& p, bool selected)
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

    void drawLine(sf::RenderTarget& target, const math::Point2f& a, const math::Point2f& b, sf::Color color)
    {
        drawLine(target, a.x, a.y, b.x, b.y, color);
    }

    void drawLine(sf::RenderTarget& target, float x1, float y1, float x2, float y2, sf::Color color)
    {
        sf::Vertex v[] = {
            sf::Vertex(sf::Vector2f(x1, y1), color),
            sf::Vertex(sf::Vector2f(x2, y2), color),
        };
        target.draw(v, 2, sf::Lines);
    }

    void drawArrow(sf::RenderTarget& target, float x1, float y1, float x2, float y2, sf::Color color)
    {
        auto len = math::pointDistance(x1, y1, x2, y2);
        sf::Transform trans;
        trans.translate(x1, y1);
        trans.rotate(math::Vec2f(x2 - x1, y2 - y1).angle());

        sf::Vertex v[] = {
            sf::Vertex(sf::Vector2f(0, 0), color),
            sf::Vertex(sf::Vector2f(len, 0), color),
            sf::Vertex(sf::Vector2f(len, 0), color),
            sf::Vertex(sf::Vector2f(len - 5, -5), color),
            sf::Vertex(sf::Vector2f(len, 0), color),
            sf::Vertex(sf::Vector2f(len - 5, 5), color),
        };
        target.draw(v, 6, sf::Lines, trans);
    }

    void drawDragBoxes(sf::RenderTarget& target, const math::Polygon<float>& pol, size_t selected)
    {
        for (size_t i = 0; i < pol.size(); ++i)
            drawDragBox(target, pol.get(i), i == selected);
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
                        pol->getPolygon().foreachSegment([&](const math::Line2f seg) {
                                vertices.append(sf::Vertex(sf::Vector2f(seg.p.x, seg.p.y), col));
                                vertices.append(sf::Vertex(sf::Vector2f(seg.p.x + seg.d.x, seg.p.y + seg.d.y), col));
                                return false;
                            });
                    }
                    else
                    {
                        auto& rect = comp->getBBox();
                        vertices.append(sf::Vertex(sf::Vector2f(rect.x, rect.y), col));
                        vertices.append(sf::Vertex(sf::Vector2f(rect.x + rect.w, rect.y), col));

                        vertices.append(sf::Vertex(sf::Vector2f(rect.x + rect.w, rect.y), col));
                        vertices.append(sf::Vertex(sf::Vector2f(rect.x + rect.w, rect.y + rect.h), col));

                        vertices.append(sf::Vertex(sf::Vector2f(rect.x, rect.y), col));
                        vertices.append(sf::Vertex(sf::Vector2f(rect.x, rect.y + rect.h), col));

                        vertices.append(sf::Vertex(sf::Vector2f(rect.x, rect.y + rect.h), col));
                        vertices.append(sf::Vertex(sf::Vector2f(rect.x + rect.w, rect.y + rect.h), col));
                    }
                }
                return false;
            });

        target.draw(vertices);
    }

    void drawNormals(sf::RenderTarget& target, const math::Polygon<float>& pol, sf::Color col)
    {
        sf::VertexArray vertices(sf::Lines);

        auto cb = [&](const math::Line2f seg) {
            auto normal = seg.d.normalized().left();
            math::Vec2f start = seg.p.asVector() + seg.d / 2;
            math::Vec2f stop;

            if (pol.normaldir == math::NormalLeft)
                stop = start + normal * 5;
            else if (pol.normaldir == math::NormalRight)
                stop = start - normal * 5;
            else
            {
                stop = start + normal * 5;
                start -= normal * 5;
            }
            vertices.append(sf::Vertex(convert(start), col));
            vertices.append(sf::Vertex(convert(stop), col));
            return false;
        };
        pol.foreachSegment(cb);
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
