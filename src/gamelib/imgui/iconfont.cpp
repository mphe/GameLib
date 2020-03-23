#include "gamelib/imgui/iconfont.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

namespace gamelib
{
    std::string findFontPath(const std::string& name)
    {
        auto resmgr = ResourceManager::getActive();
        if (resmgr)
        {
            auto font = resmgr->findFile(name);
            if (!font.empty())
            {
                LOG("Resolved icon font to path ", font);
                return font.string();
            }
        }
        LOG_ERROR("Could not resolve font file for '", name, "'");
        return name;
    }

    void setupIconFont()
    {
        ImGuiIO& io = ImGui::GetIO();

        // Don't add new font, use default font if it exists
        if (io.Fonts->Fonts.size() == 0)
            io.Fonts->AddFontDefault();

        // merge in icons from Font Awesome
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        // icons_config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced

        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        io.Fonts->AddFontFromFileTTF(findFontPath(FONT_ICON_FILE_NAME_FAS).c_str(), 13.0f, &icons_config, icons_ranges);
        // use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid

        ImGui::SFML::UpdateFontTexture();
    }
}
