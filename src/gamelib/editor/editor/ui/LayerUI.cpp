#include "editor/editor/ui/LayerUI.hpp"
#include "editor/editor/ui/inputs.hpp"
#include "gamelib/utils/string.hpp"
#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    void drawLayerUI()
    {
        static Layer::Handle current;
        static char newlayerbuf[256];
        static bool exists = false;

        Scene& scene = *Scene::getActive();
        Layer* layer = nullptr;

        // Layer listbox with checkboxes to toggle visibility
        // TODO: Maybe open an issue on imgui's github page related to
        //       proper scaling when a listbox contains not only text
        if (ImGui::ListBoxHeader("Layers", -1, 7))
        {
            scene.foreachLayer([&](Layer::Handle handle, Layer& layer) {
                ImGui::PushID(layer.getName().c_str());
                bool visible = !(layer.flags & render_invisible);
                if (ImGui::Checkbox("##layervis", &visible))
                    TOGGLEFLAG(layer.flags, render_invisible);

                ImGui::SameLine();

                if (ImGui::Selectable(layer.getName().c_str(), (handle == current)))
                    current = handle;

                ImGui::PopID();
            });
            ImGui::ListBoxFooter();
        }

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

        ImGui::Separator();

        // Layer properties
        layer = scene.getLayer(current);
        if (layer)
        {
            // ImGui::Text("Name: %s", layer->getName().c_str());
            inputSceneData(*layer);
        }

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

    bool inputLayer(const std::string& label, Layer::Handle* handle)
    {
        int selected = 0;
        auto scene = Scene::getActive();
        std::vector<Layer::Handle> cache;

        int i = 1;
        cache.push_back(Layer::Handle());
        scene->foreachLayer([&](Layer::Handle h, Layer&) {
                if (handle && h == *handle)
                    selected = i;
                cache.push_back(h);
                ++i;
            });

        auto itemgetter = [](void* cache_, int index, const char** name) {
            auto& handles = *static_cast<decltype(cache)*>(cache_);
            if (handles[index].isNull())
                *name = "default";
            else
                *name = Scene::getActive()->getLayer(handles[index])->getName().c_str();
            return true;
        };

        if (ImGui::Combo(label.c_str(), &selected, itemgetter, (void*)&cache, cache.size()))
        {
            *handle = cache[selected];
            return true;
        }

        return false;
    }
}
