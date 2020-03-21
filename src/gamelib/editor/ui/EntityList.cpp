#include "gamelib/editor/ui/EntityList.hpp"
#include "gamelib/imgui/buttons.hpp"
#include "gamelib/imgui/inputs.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/editor/tools/SelectTool.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/core/ecs/serialization.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include <SFML/Window/Mouse.hpp>

namespace gamelib
{
    void drawSearchPopup(bool* open)
    {
        static ImGuiTextFilter filter;
        static EntityReference ent;

        auto close = [&]() {
            *open = false;
            ent = nullptr;
            filter.Clear();
        };

        if (ImGui::Begin("Find entity", open, ImGuiWindowFlags_AlwaysAutoResize))
        {
            SelectTool& select = EditorShared::getSelectTool();
            auto entmgr = getSubsystem<EntityManager>();

            if (ImGui::IsWindowAppearing())
                ImGui::SetKeyboardFocusHere();

            if (filter.Draw())
                ent = entmgr->foreach([&](EntityReference ent) {
                    return filter.PassFilter(ent->getName().c_str());
                });

            ImGui::SameLine();

            if (okButton("Select"))
            {
                if (ent)
                {
                    select.select(ent);
                    if (ent && open)
                        close();
                }
                else
                    ImGui::SetKeyboardFocusHere(-1);
            }

            ImGui::SameLine();

            if (open && cancelButton("Cancel"))
                close();

            if (!ent)
                ImGui::TextColored(sf::Color::Red, "No entity found");
            else
                ImGui::Text("%s", ent->getName().c_str());
        }
        ImGui::End();
    }

    bool drawEntityNode(EntityReference ent, EntityReference current, const ImGuiTextFilter& filter)
    {
        SelectTool& select = EditorShared::getSelectTool();
        auto entname = ent->getName().c_str();
        bool ret = false;

        ImGui::PushID(ent.get());
        auto flags = current == ent ? ImGuiTreeNodeFlags_Selected : 0;
        flags |= ent->getChildren().empty() ?  ImGuiTreeNodeFlags_Leaf : 0;
        bool open = ImGui::TreeNodeEx(entname,
                flags | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow);

        if (ImGui::BeginPopupContextItem("Context Menu##enttree", sf::Mouse::Right))
        {
            if (ImGui::Selectable("Ungroup"))
                while (!ent->getChildren().empty())
                    ent->getChildren()[0]->reparent(ent->getParent());
            ImGui::EndPopup();
        }

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoHoldToOpenOthers))
        {
            ImGui::SetDragDropPayload("dnd_entity_tree", &ent, sizeof(ent));
            ImGui::EndDragDropSource();
        }
        else
        {
            const ImGuiPayload* payload = ImGui::GetDragDropPayload();
            if (payload)
            {
                EntityReference child = *static_cast<EntityReference*>(payload->Data);

                if (child != ent && !ent->isChildOf(child) && ImGui::BeginDragDropTarget())
                {
                    if (ImGui::AcceptDragDropPayload("dnd_entity_tree"))
                    {
                        child->reparent(ent);
                        ret = true;
                    }
                    ImGui::EndDragDropTarget();
                }
            }
            else if (!ImGui::IsMouseDragging() && ImGui::IsItemHovered())
            {
                if (ImGui::IsMouseReleased(0))
                    select.select(ent);
            }
        }

        if (open)
        {
            for (auto& i : ent->getChildren())
                if (drawEntityNode(i.get(), current, filter))
                {
                    ret = true;
                    break;
                }

#if 0
            // Show Components
            for (auto& comp : *ent)
            {
                ImGui::PushID(comp.ptr.get());
                auto name = generateName(comp.ptr->getName(), comp.id);
                flags = select.getSelectedComponent() == comp.ptr.get()? ImGuiTreeNodeFlags_Selected : 0;
                ImGui::TreeNodeEx(name.c_str(), flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
                if (ImGui::IsItemClicked())
                    select.selectComponent(comp.ptr.get());
                ImGui::PopID();
            }
#endif

            ImGui::TreePop();
        }

        ImGui::PopID();
        return ret;
    }

    void drawEntityList(bool* open)
    {
        auto entmgr = getSubsystem<EntityManager>();
        if (!entmgr)
            return;

        EntityReference current = EditorShared::getSelected();

        if (ImGui::Begin("Entities", open, ImVec2(250, 285)))
        {
            static ImGuiTextFilter filter;

            ImGui::PushItemWidth(0.5 * ImGui::GetWindowContentRegionWidth());
            filter.Draw();
            ImGui::PopItemWidth();

            for (auto& i : entmgr->getRoot()->getChildren())
            {
                // if (!filter.PassFilter(i.getName().c_str()))
                //     continue;

                if (drawEntityNode(i.get(), current, filter))
                    break;
            }
        }
        ImGui::End();
    }

    void drawEntityProperties(bool* open)
    {
        SelectTool& select = EditorShared::getSelectTool();
        auto ent = select.getSelected();
        auto comp = select.getSelectedComponent();

        if (ImGui::Begin(entity_properties_window_name, open, ImVec2(250, 285)))
        {
            if (comp)
                inputComponent(*comp);
            else if (ent)
                inputEntityProps(*ent);
        }
        ImGui::End();
    }
}
