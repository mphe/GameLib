#include "editor/editor/ui/props.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/ecs/RenderComponent.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/core/geometry/flags.hpp"
#include "gamelib/components/update/QPhysics.hpp"
#include "gamelib/components/update/QController.hpp"
#include "editor/components/BrushComponent.hpp"
#include "gamelib/components/rendering/PolygonShape.hpp"
#include "editor/editor/EditorShared.hpp"
#include "editor/editor/ui/LayerUI.hpp"
#include "editor/editor/ui/resources.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

namespace gamelib
{
    void drawEntityProps(Entity& ent)
    {
        static std::string jsonstring;

        auto brush = ent.find<BrushComponent>();

        if (ImGui::CollapsingHeader("Geometry", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto pos = ent.getTransform().getPosition();
            auto scale = ent.getTransform().getScale();

            if (ImGui::InputFloat2("Position", &pos[0], 2))
                ent.getTransform().setPosition(pos);

            if (ImGui::InputFloat2("Scale", &scale[0], 2))
                ent.getTransform().setScale(scale);
        }

        if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::SliderFloat("Gravity", &QPhysics::gravity, 0, 10000);
            ImGui::SliderFloat("Friction", &QPhysics::friction, 0, 10);
            ImGui::SliderFloat("Stop Speed", &QPhysics::stopSpeed, 1, 500);
            ImGui::SliderFloat("Stop Friction", &QPhysics::stopFriction, 0, QPhysics::stopSpeed);

            auto qphys = (QPhysics*)ent.find(QPhysics::name);
            auto qctrl = (QController*)ent.find(QController::name);
            if (qphys)
            {
                ImGui::Separator();
                ImGui::Text("Instance properties");
                ImGui::SliderFloat("Overbounce", &qphys->overbounce, 1, 2);
            }
            if (qctrl)
            {
                ImGui::SliderFloat("Maxspeed", &qctrl->maxspeed, 1, 500);
                ImGui::SliderFloat("Jumpspeed", &qctrl->jumpspeed, 1, 500);
                ImGui::SliderFloat("Accelerate", &qctrl->accelerate, 1, 100);
                ImGui::SliderFloat("Air Accelerate", &qctrl->airAccelerate, 1, 100);
            }
        }

        if (brush && ImGui::CollapsingHeader("Brush properties", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);

            PolygonShape* shape = brush->getBrushShape();
            auto offset = shape->getTexOffset();
            auto width = brush->getWidth();

            drawTextureSelect(&shape->texture);

            if (ImGui::InputFloat2("Texture offset", &offset[0], 2) && shape)
                shape->setTexOffset(offset);

            if (ent.getName() == "linebrush")
                if (ImGui::InputInt("Line width", &width, 1, 32))
                    brush->setWidth(width);

            ImGui::Checkbox("Export as sprite", &brush->exportAsSprite);

            if (brush->exportAsSprite)
            {
                static int exportmode = 0;
                ImGui::Indent(16);
                ImGui::RadioButton("Keep collisions", &exportmode, BrushComponent::KeepCollisions);
                ImGui::RadioButton("Drop collisions", &exportmode, BrushComponent::DropCollisions);
                ImGui::RadioButton("Pixel perfect collisions", &exportmode, BrushComponent::UsePixelCollisions);
                ImGui::Unindent(16);
                brush->exportMode = static_cast<BrushComponent::ExportMode>(exportmode);
            }

            ImGui::PopItemWidth();
        }

        if (ImGui::CollapsingHeader("Rendering", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ent.findAll<RenderComponent>([&](RenderComponent* comp) {
                ImGui::PushID(comp);
                if (ImGui::TreeNode(comp->getName().c_str()))
                {
                    auto& ren = *ent.find<RenderComponent>();
                    int depth = ren.getDepth();
                    float parallax = ren.getParallax();

                    ImGui::CheckboxFlags("Hidden", &ren.flags, render_hidden);
                    ImGui::CheckboxFlags("Disable parallax", &ren.flags, render_noparallax);
                    ImGui::CheckboxFlags("Wireframe", &ren.flags, render_wireframe);

                    if (ImGui::InputInt("Depth", &depth, 1, 100))
                        ren.setDepth(depth);

                    if (ImGui::InputFloat("Parallax", &parallax, 0.01, 0.1, 3))
                        ren.setParallax(parallax);

                    auto handle = EditorShared::getLayerUI().drawSelector("Layer", ren.getLayer());
                    if (!handle.isNull())
                        ren.setLayer(handle);

                    ImGui::TreePop();
                }

                ImGui::PopID();
                return false;
            });
        }

        if (ImGui::CollapsingHeader("Collision", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ent.findAll<CollisionComponent>([&](CollisionComponent* comp) {
                    ImGui::PushID(comp);
                    if (ImGui::TreeNode(comp->getName().c_str()))
                    {
                        ImGui::CheckboxFlags("Solid", &comp->flags, collision_solid);
                        ImGui::TreePop();
                    }
                    ImGui::PopID();
                    return false;
                });
        }
    }
}
