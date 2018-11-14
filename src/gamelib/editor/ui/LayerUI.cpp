#include "gamelib/editor/ui/LayerUI.hpp"
#include "gamelib/imgui/inputs.hpp"
#include "gamelib/imgui/buttons.hpp"
#include "gamelib/utils/string.hpp"
#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    void drawLayerUI(bool* open)
    {
        static Layer::Handle current;
        static char newlayerbuf[256];
        static bool exists = false;

        if (ImGui::Begin("Layers", open, ImVec2(250, 285)))
        {
            Scene& scene = *Scene::getActive();
            Layer* layer = nullptr;

            ImGui::Columns(2);

            // Layer listbox with checkboxes to toggle visibility
            // TODO: Maybe open an issue on imgui's github page related to
            //       proper scaling when a listbox contains not only text
            ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
            if (ImGui::ListBoxHeader("##Layers", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing())))
            {
                scene.foreachLayer([&](Layer::Handle handle, Layer& layer) {
                        ImGui::PushID(layer.getName().c_str());
                        bool visible = !(layer.flags & render_invisible);
                        if (ImGui::Checkbox("##layervis", &visible))
                            TOGGLEFLAG(layer.flags, render_invisible);

                        ImGui::SameLine();

                        if (ImGui::Selectable(layer.getName().c_str(), (handle == current)) || current.isNull())
                            current = handle;

                        ImGui::PopID();
                    });
                ImGui::ListBoxFooter();
            }
            ImGui::PopItemWidth();

            if (ImGui::Button("New Layer"))
            {
                ImGui::OpenPopup("Create new layer");
                memset(newlayerbuf, 0, sizeof(newlayerbuf));
                exists = false;
            }

            ImGui::SameLine();

            if (ImGui::Button("Delete") && !current.isNull())
            {
                scene.deleteLayer(current);
                current = Layer::Handle();
            }

            // ImGui::Separator();
            ImGui::NextColumn();

            // Layer properties
            layer = scene.getLayer(current);
            if (layer)
            {
                // ImGui::Text("Name: %s", layer->getName().c_str());
                inputSceneData(*layer);
            }

            ImGui::Columns(1);

            if (ImGui::BeginPopupModal("Create new layer", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                if (!ImGui::IsAnyItemActive())
                    ImGui::SetKeyboardFocusHere();

                if (ImGui::InputText("Name", newlayerbuf, sizeof(newlayerbuf)))
                {
                    exists = false;
                    scene.foreachLayer([&](Layer::Handle, Layer& layer) {
                            if (layer.getName() == newlayerbuf)
                                exists = true;
                        });
                }

                if (exists)
                    ImGui::TextColored(sf::Color::Red, "A layer with that name already exists.");

                ImGui::Columns(2, nullptr, false);

                if (okButton("Create") && strlen(newlayerbuf) > 0 && !exists)
                {
                    scene.createLayer(std::string(newlayerbuf));
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
