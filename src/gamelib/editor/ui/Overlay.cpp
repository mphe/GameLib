#include "gamelib/editor/ui/Overlay.hpp"
#include "gamelib/editor/tools/ToolUtils.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/ecs/RenderComponent.hpp"
#include "gamelib/core/geometry/flags.hpp"
#include "gamelib/core/input/InputSystem.hpp"
#include "gamelib/components/geometry/Polygon.hpp"
#include "gamelib/components/update/QPhysics.hpp"
#include "imgui.h"

namespace gamelib
{
    Overlay::Overlay() :
        renderSolid(true),
        renderNormals(true),
        renderVel(true),
        showCoords(false),
        wireframe(false)
    { }

    void Overlay::render(sf::RenderTarget& target)
    {
        auto input = getSubsystem<InputSystem>();
        if (!input->isMouseConsumed() && showCoords)
        {
            auto mouse = EditorShared::isSnapEnabled() ? EditorShared::getMouseSnapped() : EditorShared::getMouse();
            ImGui::BeginTooltip();
            ImGui::SetTooltip("%i, %i", (int)mouse.x, (int)mouse.y);
            ImGui::EndTooltip();
        }

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

        if (wireframe)
        {
            auto wirerenderer = [&](RenderComponent* ren) {
                auto& mesh = ren->getVertices();
                if (mesh.getPrimitiveType() == sf::TriangleStrip)
                {
                    if (mesh.getVertexCount() <= 2)
                        return false;

                    sf::Vertex line[3];
                    line[0] = sf::Vertex(mesh[0].position);
                    line[1] = sf::Vertex(mesh[1].position);
                    target.draw(line, 2, sf::LineStrip, ren->getTransform());

                    for (size_t i = 2; i < mesh.getVertexCount(); ++i)
                    {
                        line[0] = sf::Vertex(mesh[i - 2].position);
                        line[1] = sf::Vertex(mesh[i].position);
                        line[2] = sf::Vertex(mesh[i - 1].position);
                        target.draw(line, 3, sf::LineStrip, ren->getTransform());
                    }
                }
                else if (mesh.getPrimitiveType() == sf::Triangles || mesh.getPrimitiveType() == sf::Quads)
                {
                    size_t num = mesh.getPrimitiveType() == sf::Triangles ? 3 : 4;
                    if (mesh.getVertexCount() < num)
                        return false;

                    for (size_t i = 0; i < mesh.getVertexCount(); i += num)
                    {
                        sf::Vertex line[num + 1];
                        for (size_t n = 0; n < num; ++n)
                            line[n] = sf::Vertex(mesh[i + n].position);
                        line[num] = sf::Vertex(mesh[i].position);
                        target.draw(line, num + 1, sf::LineStrip, ren->getTransform());
                    }
                }
                else
                    target.draw(&mesh[0], mesh.getVertexCount(), mesh.getPrimitiveType(), ren->getTransform());

                return false;
            };

            ent->findAllByType<RenderComponent>(wirerenderer);
        }
    }
}
