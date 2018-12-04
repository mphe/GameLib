#include "gamelib/editor/ui/Overlay.hpp"
#include "gamelib/editor/tools/ToolUtils.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/components/RenderComponent.hpp"
#include "gamelib/core/geometry/flags.hpp"
#include "gamelib/core/input/InputSystem.hpp"
#include "gamelib/core/Game.hpp"
#include "gamelib/core/rendering/Scene.hpp"
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
        wireframe(false),
        debugOverlay(true),
        debugOverlayMovable(true),
        renderCams(true)
    { }

    void Overlay::drawDebugOverlay()
    {
        if (debugOverlay)
        {
            if (!debugOverlayMovable)
                ImGui::SetNextWindowPos(ImVec2(0, 16));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            if (ImGui::Begin("Stats overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
            {
                auto game = getSubsystem<Game>();
                auto frametime = game->getFrametime();
                ImGui::Text("FPS: %i", (int)(frametime != 0 ? 1.0 / frametime : 0));
                ImGui::Text("Frametime: %i ms", (int)(frametime * 1000));
                ImGui::Text("Real frametime: %i ms", (int)(game->getRealFrametime() * 1000));
                ImGui::Text("Render time: %f ms", game->getRenderTime() * 1000);
                ImGui::Text("Update time: %f ms", game->getUpdateTime() * 1000);
                ImGui::Text("Objects rendered: %lu", getSubsystem<Scene>()->getNumObjectsRendered());
            }
            ImGui::End();
            ImGui::PopStyleColor();
        }
    }

    void Overlay::drawGui()
    {
        auto input = getSubsystem<InputSystem>();
        if (!input->isMouseConsumed() && showCoords)
        {
            auto mouse = EditorShared::isSnapEnabled() ? EditorShared::getMouseSnapped() : EditorShared::getMouse();
            ImGui::BeginTooltip();
            ImGui::SetTooltip("%i, %i", (int)mouse.x, (int)mouse.y);
            ImGui::EndTooltip();
        }
    }

    void Overlay::render(sf::RenderTarget& target)
    {
        if (renderCams)
        {
            auto scene = getSubsystem<Scene>();
            sf::Color col = sf::Color::Green;

            for (size_t i = 0; i < scene->getNumCameras(); ++i)
            {
                auto cam = scene->getCamera(i);
                math::AABBf baserect(cam->pos.asPoint(), cam->size);
                drawRectOutline(target, baserect, col);

                if (!math::almostEquals(cam->zoom, 1.f))
                {
                    math::AABBf rect = cam->getCamRect();
                    drawRectOutline(target, rect, col);
                    drawLine(target, rect.pos.asPoint(), baserect.pos.asPoint(), col);
                    drawLine(target, math::Point2f(rect.x + rect.w, rect.y), math::Point2f(baserect.x + baserect.w, baserect.y), col);
                    drawLine(target, math::Point2f(rect.x, rect.y + rect.h), math::Point2f(baserect.x, baserect.y + baserect.h), col);
                    drawLine(target, rect.pos.asPoint() + rect.size, baserect.pos.asPoint() + baserect.size, col);
                }
            }
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
                    target.draw(line, 2, sf::LineStrip, ren->getMatrix());

                    for (size_t i = 2; i < mesh.getVertexCount(); ++i)
                    {
                        line[0] = sf::Vertex(mesh[i - 2].position);
                        line[1] = sf::Vertex(mesh[i].position);
                        line[2] = sf::Vertex(mesh[i - 1].position);
                        target.draw(line, 3, sf::LineStrip, ren->getMatrix());
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
                        target.draw(line, num + 1, sf::LineStrip, ren->getMatrix());
                    }
                }
                else
                    target.draw(&mesh[0], mesh.getVertexCount(), mesh.getPrimitiveType(), ren->getMatrix());

                return false;
            };

            ent->findAllByType<RenderComponent>(wirerenderer);
        }
    }
}
