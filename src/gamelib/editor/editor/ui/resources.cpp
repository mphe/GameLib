#include "editor/editor/ui/resources.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

namespace gamelib
{
    const char* popupTextureSelect = "Select Texture";


    bool drawTextureSelect(TextureResource::Handle* tex)
    {
        ImGui::Text("Texture");
        if (*tex)
        {
            if (ImGui::ImageButton(**tex, ImVec2(32, 32)))
                ImGui::OpenPopup(popupTextureSelect);
            ImGui::SameLine();
        }

        if (ImGui::Button("Change"))
            ImGui::OpenPopup(popupTextureSelect);

        // Select texture popup
        auto newtex = drawTextureSelectPopup();
        if (newtex)
        {
            *tex = newtex;
            return true;
        }
        return false;
    }

    TextureResource::Handle drawTextureSelectPopup()
    {
        using namespace gamelib;

        TextureResource::Handle  ptr;

        if (ImGui::BeginPopupModal(popupTextureSelect))
        {
            float w = ImGui::GetWindowContentRegionWidth();
            constexpr float size = 72;

            ResourceManager::getActive()->foreach([&](const std::string& fname, BaseResourceHandle res) {
                    TextureResource::Handle tex = res.as<TextureResource>();
                    if (ImGui::ImageButton(*tex, sf::Vector2f(size, size)))
                    {
                        ptr = tex;
                        ImGui::CloseCurrentPopup();
                    }

                    w -= size;
                    if (w > size)
                        ImGui::SameLine();
                    else
                        w = ImGui::GetWindowContentRegionWidth();
                }, TextureResource::id);

            ImGui::NewLine();

            if (ImGui::Button("Cancel"))
                ImGui::CloseCurrentPopup();

            ImGui::EndPopup();
        }

        return ptr;
    }
}
