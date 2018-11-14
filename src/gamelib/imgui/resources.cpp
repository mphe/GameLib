#include "gamelib/imgui/resources.hpp"
#include "gamelib/imgui/buttons.hpp"
#include "gamelib/core/res/SpriteResource.hpp"
#include "gamelib/core/res/JsonResource.hpp"
#include "gamelib/core/res/EntityConfigResource.hpp"
#include "gamelib/core/Game.hpp"
#include "imgui.h"
#include "imgui_internal.h" // for imgui context
#include "imgui-SFML.h"

namespace gamelib
{
    bool getThumbnailTexture(BaseResourceHandle res, sf::Sprite* sprite)
    {
        if (res)
            sprite->setTexture(*res.as<TextureResource>(), true);
        return (bool)res;
    }

    bool getThumbnailSprite(BaseResourceHandle res, sf::Sprite* sprite)
    {
        if (res)
        {
            auto sprres = res.as<SpriteResource>();
            sprite->setTexture(*sprres->tex);
            sprite->setTextureRect(sf::IntRect(sprres->rect.x, sprres->rect.y, sprres->rect.w, sprres->rect.h));
        }
        return (bool)res;
    }

    void previewTexture(BaseResourceHandle res)
    {
        previewTexture(res, -1);
    }

    void previewSprite(BaseResourceHandle res)
    {
        previewSprite(res, -1);
    }

    void previewJson(BaseResourceHandle res)
    {
        static BaseResource* last = nullptr;
        static std::string cache;

        if (res)
        {
            // Update cache if resource changed
            if (!last || last != res.getResource())
            {
                last = res.getResource();
                cache = res.as<JsonResource>()->toStyledString();
            }

            ImGui::Text("%s", cache.c_str());
        }
    }


    void previewTexture(BaseResourceHandle res, int imgsize)
    {
        if (res)
        {
            auto tex = res.as<TextureResource>();
            auto w = (imgsize == -1) ? ImGui::GetContentRegionAvailWidth() : imgsize;
            auto h = w * ((float)tex->getSize().y / tex->getSize().x);
            ImGui::Image(*tex, ImVec2(w, h));
            ImGui::Text("Resolution: %ux%upx", tex->getSize().x, tex->getSize().y);
        }
    }

    void previewSprite(BaseResourceHandle res, int imgsize)
    {
        static BaseResource* last = nullptr;
        static AnimationData ani;
        static bool paused = false;

        if (res)
        {
            auto sprite = res.as<SpriteResource>();

            // Update sprite if resource changed
            if (!last || last != res.getResource())
            {
                last = res.getResource();
                ani = sprite->ani;
            }

            if (!paused)
                ani.update(getSubsystem<Game>()->getFrametime());

            ImGuiContext& g = *ImGui::GetCurrentContext();
            auto size = g.FontSize + g.Style.FramePadding.y * 2.0f;
            auto bsize = ImVec2(size, size);

            auto rect = getSpriteRect(ani.offset, sprite->rect, sprite->tex->getSize().x, sprite->tex->getSize().y);
            auto w = (imgsize == -1) ? ImGui::GetContentRegionAvailWidth() * 0.8 : imgsize;
            auto h = w * ((float)rect.h / rect.w);
            ImGui::Image(*sprite->tex, ImVec2(w, h), sf::FloatRect(rect.x, rect.y, rect.w, rect.h));

            { // Animation controls
                if (paused && ImGui::Button(">", bsize))
                    paused = false;
                else if (!paused && ImGui::Button("||", bsize))
                    paused = true;

                ImGui::SameLine();
                ImGui::Spacing();
                ImGui::SameLine();

                ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5);
                int frame = ani.offset;
                if (ImGui::SliderInt("Frame", &frame, 0, ani.length - 1))
                    ani.offset = frame;
                ImGui::PopItemWidth();

                ImGui::SameLine();
                ImGui::Spacing();
                ImGui::SameLine();

                if (ImGui::Button("+", bsize))
                    ani.setIndex(ani.offset + 1);
                ImGui::SameLine();
                if (ImGui::Button("-", bsize))
                    ani.setIndex(ani.offset - 1);
            }

            ImGui::Text("Length: %i", ani.length);
            ImGui::Text("Speed: %.2f", ani.speed);
            ImGui::Text("Texture: %s", sprite->tex.getResource()->getPath().c_str());
            ImGui::Text("Resolution: %ix%ipx", sprite->rect.w, sprite->rect.h);
            ImGui::Text("Offset: X: %ipx  Y: %ipx", sprite->rect.x, sprite->rect.y);
        }
    }


    void getResourceMeta(ID id, const char** name, ThumbnailFunction* thumbgetter, PreviewFunction* preview)
    {
        const char* tmpname;
        ThumbnailFunction tmpthumb = nullptr;
        PreviewFunction tmppreview = nullptr;

        switch (id)
        {
            case TextureResource::id:
                tmpname = "Texture";
                tmpthumb = getThumbnailTexture;
                tmppreview = previewTexture;
                break;
            case SpriteResource::id:
                tmpname = "Sprite";
                tmpthumb = getThumbnailSprite;
                tmppreview = previewSprite;
                break;
            case JsonResource::id:
                tmpname = "Json";
                tmppreview = previewJson;
                break;
            default:
                tmpname = "Other";
                break;
        }

        if (name)
            *name = tmpname;
        if (thumbgetter)
            *thumbgetter = tmpthumb;
        if (preview)
            *preview = tmppreview;
    }


    bool inputResource(const char* name, BaseResourceHandle* res, ID id, bool preview_, int size)
    {
        ThumbnailFunction thumbgetter = nullptr;
        PreviewFunction preview = nullptr;

        getResourceMeta(id, nullptr, &thumbgetter, preview_ ? &preview : nullptr);

        return inputResource(name, res, id, thumbgetter, preview, size);
    }

    bool inputResource(const char* name, BaseResourceHandle* res, ID id, ThumbnailFunction thumbgetter, PreviewFunction preview, int size)
    {
        static BaseResourceHandle tmphandle; // Don't set anything until OK is pressed if preview is used

        bool selected = false;  // returned value

        ImGui::PushID(name);
        { // Widget
            bool startSelect = false;

            if (*res)
            {
                ImGui::BeginGroup();
                // ImGui::Columns(2, nullptr, false);
                // ImGui::SetColumnWidth(0, 50);

                if (thumbgetter)
                {
                    sf::Sprite thumb;
                    thumbgetter(*res, &thumb);
                    if (ImGui::ImageButton(thumb, ImVec2(32, 32)))
                        startSelect = true;

                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        ImGui::Text("%s", res->getResource()->getPath().c_str());
                        ImGui::EndTooltip();
                    }
                }
                else
                {
                    ImGui::Text("%s", res->getResource()->getPath().c_str());
                }

                ImGui::EndGroup();
                ImGui::SameLine();
                // ImGui::NextColumn();
            }
            ImGui::BeginGroup();
            // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8);
            ImGui::TextUnformatted(name);

            if (!*res)
                ImGui::SameLine();

            if (!*res && ImGui::Button("Change"))
                startSelect = true;
            else if (*res && ImGui::Button("Clear"))
            {
                res->reset();
                selected = true;
            }

            ImGui::EndGroup();

            if (startSelect)
            {
                ImGui::OpenPopup("Select Resource");
                ImGui::SetNextWindowSize(ImVec2(300, 300));
                tmphandle = *res;
            }
        }

        { // Selection Popup
            if (*res == tmphandle && ImGui::BeginPopupModal("Select Resource"))
            {
                selected = drawResourceSelection(preview ? &tmphandle : res, id, thumbgetter, preview, size);
                if (selected && !preview)
                    ImGui::CloseCurrentPopup();

                { // OK / Cancel
                    bool close = false;
                    ImGui::PushItemWidth(-140); // Align right

                    if (preview)
                    {
                        selected = false;   // Don't set anything until OK is pressed

                        if (okButton())
                        {
                            selected = true;
                            close = true;
                        }
                        ImGui::SameLine();
                    }

                    if (cancelButton())
                        close = true;

                    ImGui::PopItemWidth();

                    if (close)
                    {
                        ImGui::CloseCurrentPopup();

                        if (preview)
                        {
                            *res = tmphandle;
                            tmphandle.reset();
                        }
                    }
                }
                ImGui::EndPopup();
            }
        }

        ImGui::PopID();
        return selected;
    }


    bool drawResourceSelection(BaseResourceHandle* handle, ID id, bool preview_, int size)
    {
        ThumbnailFunction thumbgetter = nullptr;
        PreviewFunction preview = nullptr;

        getResourceMeta(id, nullptr, &thumbgetter, preview_ ? &preview : nullptr);

        return drawResourceSelection(handle, id, thumbgetter, preview, size);
    }


    bool drawResourceSelection(BaseResourceHandle* handle, ID id, ThumbnailFunction thumbgetter, PreviewFunction preview, int size)
    {
        bool selected = false;

        if (preview && *handle)
            ImGui::Columns(2);

        ImGui::BeginChild("List", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()));
        if (thumbgetter)
            selected = drawResourceSelectionGrid(handle, id, thumbgetter, size);
        else
            selected = drawResourceSelectionList(handle, id);
        ImGui::EndChild();

        if (preview && *handle)
        {
            ImGui::NextColumn();
            ImGui::BeginChild("Preview");
            preview(*handle);
            if (thumbgetter)
                ImGui::Text("File: %s", handle->getResource()->getPath().c_str());
            ImGui::EndChild();
            ImGui::Columns(1);
        }

        return selected;
    }

    bool drawResourceSelectionGrid(BaseResourceHandle* handle, ID id, ThumbnailFunction thumbgetter, int size)
    {
        bool selected = false;
        float w = ImGui::GetContentRegionAvailWidth();
        sf::Sprite thumb;

        ResourceManager::getActive()->foreach([&](const std::string& fname, BaseResourceHandle res) {
                if (thumbgetter)
                    thumbgetter(res, &thumb);

                ImGui::PushID(fname.c_str());
                if (ImGui::ImageButton(thumb, sf::Vector2f(size, size)))
                {
                    *handle = res;
                    selected = true;
                }
                ImGui::PopID();

                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("%s", res.getResource()->getPath().c_str());
                    ImGui::EndTooltip();
                }

                w -= size + GImGui->Style.ItemSpacing.x + GImGui->Style.FramePadding.x * 2;
                if (w > size)
                    ImGui::SameLine();
                else
                    w = ImGui::GetContentRegionAvailWidth();
                return false;
                }, id);

        ImGui::NewLine();

        return selected;
    }

    bool drawResourceSelectionList(BaseResourceHandle* handle, ID id)
    {
        static int current = 0;

        int i = 0;
        bool selected = false;
        ResourceManager::getActive()->foreach([&](const std::string& fname, BaseResourceHandle res) {
                if (ImGui::Selectable(fname.c_str(), current == i))
                {
                    current = i;
                    *handle = res;
                    selected = true;
                }
                ++i;
                return false;
        }, id);

        return selected;
    }
}
