#include "gamelib/editor/ui/CameraUI.hpp"
#include "gamelib/core/rendering/CameraSystem.hpp"
#include "gamelib/imgui/buttons.hpp"
#include "gamelib/imgui/inputs.hpp"
#include "imgui.h"

namespace gamelib
{
    // Basically the same code as LayerUI.
    // If you make changes here, do it also in LayerUI.cpp.
    void drawCameraUI(bool* open)
    {
        static Camera* current;
        static char namebuf[256];
        static bool exists = false;

        if (ImGui::Begin("Cameras", open, ImVec2(250, 285)))
        {
            CameraSystem* sys = CameraSystem::getActive();

            if (!sys)
            {
                ImGui::Text("Error: No CameraSystem available");
                return;
            }

            ImGui::Columns(2);
            ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());

            if (ImGui::ListBoxHeader("##Cameras", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing())))
            {
                for (size_t i = 0; i < sys->size(); ++i)
                {
                    auto& cam = *sys->get(i);
                    ImGui::PushID(cam.getName().c_str());
                    ImGui::Checkbox("##cameravis", &cam.active);
                    ImGui::SameLine();
                    if (ImGui::Selectable(cam.getName().c_str(), &cam == current) || !current)
                        current = &cam;
                    ImGui::PopID();
                }
                ImGui::ListBoxFooter();
            }
            ImGui::PopItemWidth();

            if (ImGui::Button("New Camera"))
            {
                ImGui::OpenPopup("Create new camera");
                memset(namebuf, 0, sizeof(namebuf));
                exists = false;
            }

            ImGui::SameLine();

            if (ImGui::Button("Delete") && current)
            {
                sys->remove(current);
                current = nullptr;
            }

            ImGui::NextColumn();

            // Camera properties
            if (current)
            {
                ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5);
                inputCamera(*current);
                ImGui::PopItemWidth();
            }

            ImGui::Columns(1);

            if (ImGui::BeginPopupModal("Create new camera", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                if (!ImGui::IsAnyItemActive())
                    ImGui::SetKeyboardFocusHere();

                if (ImGui::InputText("Name", namebuf, sizeof(namebuf)))
                {
                    exists = false;
                    for (size_t i = 0; i < sys->size(); ++i)
                        if (sys->get(i)->getName() == namebuf)
                            exists = true;
                }

                if (exists)
                    ImGui::TextColored(sf::Color::Red, "A camera with that name already exists.");

                ImGui::Columns(2, nullptr, false);

                if (okButton("Create") && strlen(namebuf) > 0 && !exists)
                {
                    sys->create(std::string(namebuf));
                    ImGui::CloseCurrentPopup();
                }

                ImGui::NextColumn();

                if (cancelButton("Cancel"))
                    ImGui::CloseCurrentPopup();

                ImGui::Columns(1);
                ImGui::EndPopup();
            }

        }
        ImGui::End();
    }
}
