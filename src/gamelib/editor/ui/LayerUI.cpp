#include "gamelib/editor/ui/LayerUI.hpp"
#include "gamelib/imgui/inputs.hpp"
#include "gamelib/imgui/buttons.hpp"
#include "gamelib/core/rendering/RenderSystem.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    bool inputLayerData(RenderSystem* sys, LayerHandle handle)
    {
        const RenderLayer* layer = sys->getLayer(handle);
        if (!layer)
            return false;

        int depth = layer->depth;
        auto options = layer->options;
        bool changed = false;

        if (inputRenderOptions("Options", &options))
        {
            sys->setLayerOptions(handle, options);
            changed = true;
        }

        if (ImGui::InputInt("Depth", &depth, 1, 100))
        {
            sys->setLayerDepth(handle, depth);
            changed = true;
        }

        return changed;
    }

    // Basically the same code as CameraUI.
    // If you make changes here, do it also in CameraUI.cpp.
    void drawLayerUI(bool* open)
    {
        static LayerHandle current;
        static char namebuf[256];
        static bool exists = false;

        if (ImGui::Begin("Layers", open, ImVec2(250, 285)))
        {
            RenderSystem* sys = RenderSystem::getActive();
            if (!sys)
            {
                ImGui::Text("Error: No RenderSystem available");
                return;
            }

            ImGui::Columns(2);

            // Layer listbox with checkboxes to toggle visibility
            // TODO: Maybe open an issue on imgui's github page related to
            //       proper scaling when a listbox contains not only text
            ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
            if (ImGui::ListBoxHeader("##Layers", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing())))
            {
                for (auto it = sys->beginLayers(), end = sys->endLayers(); it != end; ++it)
                {
                    const auto& handle = it.handle();
                    const auto& layer = *it;
                    bool visible = !(layer.options.flags & render_invisible);

                    ImGui::PushID(layer.name.c_str());
                    if (ImGui::Checkbox("##layervis", &visible))
                    {
                        auto newflags = toggleflag(layer.options.flags, render_invisible);
                        sys->setLayerOptions(handle, &newflags);
                    }

                    ImGui::SameLine();

                    if (ImGui::Selectable(layer.name.c_str(), (handle == current)) || current.isNull())
                        current = handle;

                    ImGui::PopID();
                }
                ImGui::ListBoxFooter();
            }
            ImGui::PopItemWidth();

            if (ImGui::Button("New Layer"))
            {
                ImGui::OpenPopup("Create new layer");
                memset(namebuf, 0, sizeof(namebuf));
                exists = false;
            }

            ImGui::SameLine();

            if (ImGui::Button("Delete") && !current.isNull())
            {
                sys->removeLayer(current);
                current = LayerHandle();
            }

            ImGui::NextColumn();

            // Layer properties
            inputLayerData(sys, current);

            ImGui::Columns(1);

            if (ImGui::BeginPopupModal("Create new layer", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                if (!ImGui::IsAnyItemActive())
                    ImGui::SetKeyboardFocusHere();

                if (ImGui::InputText("Name", namebuf, sizeof(namebuf)))
                {
                    exists = false;
                    for (auto it = sys->beginLayers(), end = sys->endLayers(); it != end; ++it)
                        if (it->name == namebuf)
                        {
                            exists = true;
                            break;
                        }
                }

                if (exists)
                    ImGui::TextColored(sf::Color::Red, "A layer with that name already exists.");

                ImGui::Columns(2, nullptr, false);

                if (okButton("Create") && strlen(namebuf) > 0 && !exists)
                {
                    sys->createLayer(std::string(namebuf));
                    ImGui::CloseCurrentPopup();
                }

                ImGui::NextColumn();

                if (cancelButton())
                    ImGui::CloseCurrentPopup();

                ImGui::Columns(1);
                ImGui::EndPopup();
            }

        }
        ImGui::End();
    }
}
