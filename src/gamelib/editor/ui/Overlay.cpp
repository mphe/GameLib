#include "gamelib/editor/ui/Overlay.hpp"
#include "gamelib/editor/tools/ToolUtils.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/geometry/flags.hpp"
#include "gamelib/core/input/InputSystem.hpp"
#include "gamelib/core/Game.hpp"
#include "gamelib/core/rendering/CameraSystem.hpp"
#include "gamelib/core/rendering/RenderSystem.hpp"
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

                auto numrendered = getSubsystem<CameraSystem>()->getNumRendered();
                if (!numrendered)
                    numrendered = RenderSystem::getActive()->getNumObjectsRendered();
                ImGui::Text("Objects rendered: %lu", numrendered);
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
            auto camsys = getSubsystem<CameraSystem>();
            sf::Color col = sf::Color::Green;

            for (size_t i = 0; i < camsys->size(); ++i)
            {
                auto cam = camsys->get(i);
                const math::AABBf baserect(math::Point2f(), cam->getSize());
                drawRectOutline(target, baserect, col, cam->getMatrix());

                // if (!math::almostEquals(cam->getZoom(), 1.f))
                // {
                //     auto nozoomTransform = cam->getTransformation();
                //     nozoomTransform.scale.fill(1);
                //     drawRectOutline(target, baserect, col, nozoomTransform.getMatrix());
                // }
            }
        }

        const auto ent = EditorShared::getSelected();
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
            ent->findAllByName<PolygonCollider>([&](CompRef<PolygonCollider> pol) {
                if (pol->flags & collision_solid)
                    drawNormals(target, pol->getGlobal());
                return false;
            });
    }
}
