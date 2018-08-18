#include "gamelib/editor/ui/Overlay.hpp"
#include "gamelib/editor/tools/ToolUtils.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/geometry/flags.hpp"
#include "gamelib/components/geometry/Polygon.hpp"
#include "gamelib/components/update/QPhysics.hpp"

namespace gamelib
{
    Overlay::Overlay() :
        renderSolid(true),
        renderNormals(true),
        renderVel(true)
    { }

    void Overlay::render(sf::RenderTarget& target)
    {
        auto* ent = EditorShared::getSelected();
        if (!ent)
            return;

        if (renderSolid)
            drawCollisions(target, *ent, collision_solid);

        if (renderVel)
        {
            auto phys = ent->findByName<QPhysics>();
            if (phys && !phys->vel.isZero())
            {
                auto start = ent->getTransform().getBBox().getCenter();
                auto end = start + phys->vel;
                drawArrow(target, start.x, start.y, end.x, end.y, sf::Color::Cyan);
            }
        }

        if (renderNormals)
        {
            ent->findAllByName<Polygon>([&](Polygon* pol) {
                    if (pol->flags & collision_solid)
                        drawNormals(target, pol->getPolygon());
                    return false;
                });
        }
    }
}
